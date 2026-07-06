#pragma once

#include <zephyr/kernel.h>
#include <cstdint>
#include <cstring>


template <typename data_type>
class RingBuffer
{
private:
    struct k_mutex mtx_{};

    data_type *_buffer{nullptr};
    
    uint32_t _size{0};
    // head e tail indicano rispettivamente il prossimo elemento da scrivere e da leggere.
    uint32_t _head{0};
    uint32_t _tail{0};
    uint32_t _count{0};     // Contatore per contare gli elementi presenti nel buffer

    /**
     * Funzione per l'allocazione del ringbuffer
     * 
     * Ritorna true se l'allocazione è andata bene se no false.
     */
    bool _allocate(size_t size){
        if (check() && size == _size)   // controllo se si sta cercando di modificare l'allocazione ma con gli stessi valori
        {
            return true;
        }

        if (size == 0){return false;}

        _buffer = new (std::nothrow) data_type[size] {};   // Le {} inizializzano tutti i valori a 0, inoltre (std::nothrow) fa in modo che new non generi eccezioni

         if (!_buffer){return false;}

        _size = size;
        
        reset();

        return true;
        
    };

    void _deallocate(){
        if (_buffer)
        {
            delete[] _buffer;
            _buffer = nullptr;
            _size = 0;
        }
        
    }

public:
    RingBuffer(size_t size) {
        k_mutex_init(&mtx_);
        _allocate(size);
    };

    ~RingBuffer() {_deallocate();};

   /**
    * Rule of 5:
    * no copy, assignment, move, move assignment
    */
	RingBuffer(const RingBuffer &) = delete;
	RingBuffer &operator=(const RingBuffer &) = delete;
	RingBuffer(RingBuffer &&) = delete;
	RingBuffer &operator=(RingBuffer &&) = delete;
    
    bool reallocate(size_t new_size){
        k_mutex_lock(&mtx_, K_FOREVER);
        _deallocate();
        bool ok = _allocate(new_size);
        k_mutex_unlock(&mtx_);

        return ok;
    }


    
    bool pop(data_type &out) { 

        k_mutex_lock(&mtx_, K_FOREVER);
        if (_count == 0)
        {
            k_mutex_unlock(&mtx_);
            return false;
        }
        
        out = _buffer[_tail];
        _tail = (_tail + 1) % _size;
        _count--;

        k_mutex_unlock(&mtx_);

        return true; 
    }

    void push(const data_type &data){
        k_mutex_lock(&mtx_, K_FOREVER);
        _buffer[_head] = data;
        _head = (_head + 1) % _size;
        
        // Se il count è uguale alla dimensione del buffer allora il buffer è pieno e quindi bisogna spostare avanti la coda
        if (_count == _size)
        {
            _tail = (_tail + 1) % _size;
        }else{
            _count++;
        }
        k_mutex_unlock(&mtx_);
        
    }

    bool get(uint32_t idx, data_type &out ){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (idx < _size)
        {
            out = _buffer[idx];
            k_mutex_unlock(&mtx_);
            return true;
        }
        k_mutex_unlock(&mtx_);
        return false;
    }

    /**
     * Metodo che accetta un puntatore ad array e la sua lunghezza e copia gli elementi nel buffer circolare
     */
    void push_batch(const data_type* src, size_t len){
        if (!data || len == 0) return;
        k_mutex_lock(&mtx_, K_FOREVER);
        
        k_mutex_unlock(&mtx_);
    }

    /**
     * Metodo che copia nel buffer il numero di elementi specificati da len.
     * Ritorna il numero di elementi copiati
     */
    size_t drain(data_type *buffer, size_t len) {
        k_mutex_lock(&mtx_, K_FOREVER);
        if(_count == 0){
            k_mutex_unlock(&mtx_);
            return 0;   // Buffer vuoto
        }

        size_t to_copy = (len < _count) ? len : _count;     // Numero di elementi da copiare

        size_t first_part = _size - _tail;

        if (to_copy <= first_part)
        {
            memcpy(buffer, &_buffer[_tail], to_copy * sizeof(data_type));

        }else{
            memcpy(buffer, &_buffer[_tail], first_part * sizeof(data_type));

            size_t second_part = to_copy - first_part; 

            memcpy(buffer + first_part, _buffer, second_part * sizeof(data_type));

        }
        
        _tail = (_tail + to_copy) % _size;

        _count-= to_copy;

        k_mutex_unlock(&mtx_);
        return to_copy;
    }

    /** 
     * metodo che cerca di copiare tutto il buffer, se la lunghezza del buffer di destinazione non è sufficiente
     * restituirà false se no true
     */
    bool drain_all(data_type *buffer, size_t max_len){
        k_mutex_lock(&mtx_, K_FOREVER);

        if (_count == 0)
        {
            k_mutex_unlock(&mtx_);
            return true;        // buffer vuoto
        }

        size_t to_copy = (max_len < _count) ? max_len : _count;     // Numero di elementi da copiare

        size_t first_part = _size - _tail;

        if (to_copy <= first_part)
        {
            memcpy(buffer, &_buffer[_tail], to_copy * sizeof(data_type));

        }else{
            memcpy(buffer, &_buffer[_tail], first_part * sizeof(data_type));

            size_t second_part = to_copy - first_part; 

            memcpy(buffer + first_part, _buffer, second_part * sizeof(data_type));

        }
        
        _tail = (_tail + to_copy) % _size;

        _count-= to_copy;
        

        k_mutex_unlock(&mtx_);
        return _count == 0;
    }

    /** 
     * Metodo che copia nel buffer senza spostare la tail
     * utile per ispezionare senza consumare i dati
     */
    size_t peek(data_type *buffer, size_t len){
        k_mutex_lock(&mtx_, K_FOREVER);
        if(_count == 0){
            k_mutex_unlock(&mtx_);
            return 0;   // Buffer vuoto
        }

        size_t to_copy = (len < _count) ? len : _count;     // Numero di elementi da copiare

        size_t first_part = _size - _tail;

        if (to_copy <= first_part)
        {
            memcpy(buffer, &_buffer[_tail], to_copy * sizeof(data_type) );

        }else{
            memcpy(buffer, &_buffer[_tail], first_part * sizeof(data_type) );

            size_t second_part = to_copy - first_part; 

            memcpy(buffer + first_part, _buffer, second_part * sizeof(data_type));

        }

        k_mutex_unlock(&mtx_);
        return to_copy;
    }


    /** -------------------------------------------------------
     *                  Metodi di accesso
        -------------------------------------------------------*/

    uint32_t get_head_idx() {
        k_mutex_lock(&mtx_, K_FOREVER);
        uint32_t h = _head;
        k_mutex_unlock(&mtx_);
        return h;
    }
    uint32_t get_tail_idx() {
        k_mutex_lock(&mtx_, K_FOREVER);
        uint32_t t = _tail;
        k_mutex_unlock(&mtx_);
        return t;
    }
    uint32_t get_element_count() {
        k_mutex_lock(&mtx_, K_FOREVER);
        uint32_t c = _count;
        k_mutex_unlock(&mtx_);
        return c;
    }
    bool isEmpty() {
        k_mutex_lock(&mtx_, K_FOREVER);
        bool empty = (_count == 0);
        k_mutex_unlock(&mtx_);
        return empty;
    }
    bool isFull() {
        k_mutex_lock(&mtx_, K_FOREVER);
        bool full = (_count == _size);
        k_mutex_unlock(&mtx_);
        return full;
    }

    // Metodo per controllare se il buffer è correttamente allocato
    bool check() const {
        return (_buffer != nullptr) && (_size > 0);
    }

    // Metodo per resettare il buffer
    void reset(){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (_buffer != nullptr)
        {
            for (uint32_t i = 0; i < _size; i++)
            {
                _buffer[i] = {};
            }
            
            _head = 0;
            _tail = 0;
            _count = 0;
        }
        k_mutex_unlock(&mtx_);
    }

};


