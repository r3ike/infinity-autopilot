#pragma once

#include <inttypes.h>
#include <cstdio>
#include <cstring>


template <typename data_type>
class RingBuffer
{
private:
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

        _buffer = new data_type[size] {};   // Le {} inizializzano tutti i valori a 0

        // if (_buffer == nullptr){return false;} il new non restituisce nullptr ma genera un exeption

        _size = size;
        
        reset();

        return true;
        
    };

    void _deallocate(){
        if (_buffer)
        {
            delete[] _buffer;
            _buffer = nullptr;
        }
        
    }

public:
    RingBuffer(size_t size) {_allocate(size)};
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
        _deallocate();
        return _allocate(new_size);
    }
    
    bool pop(data_type &out) { 
        if (_count == 0)
        {
            return false;
        }
        
        out = _buffer[_tail];
        _tail = (_tail + 1) % _size;
        _count--;

        return true; 
    }

    uint32_t get_head_idx(){ return _head;}
    uint32_t get_tail_idx(){ return _tail;}
    uint32_t get_element_count(){return _count;}

    bool isEmpty() {return _count == 0;}
    bool isFull() {return _count == _size;}

    void push(const data_type &data){

        _buffer[_head] = data;
        _head = (_head + 1) % _size;
        
        // Se il count è uguale alla dimensione del buffer allora il buffer è pieno e quindi bisogna spostare avanti la coda
        if (_count == _size)
        {
            _tail = (_tail + 1) % _size;
        }else{
            _count++;
        }
        
    }

    // Metodo per controllare se il buffer è correttamente allocato
    bool check() const {
        return (_buffer != nullptr) && (_size > 0);
    }

    // Metodo per resettare il buffer
    void reset(){
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
        
    }

};


