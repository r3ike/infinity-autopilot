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
    uint32_t _head{0};
    uint32_t _tail{0};

    bool _first_write{true};


    /**
     * Funzione per l'allocazione del ringbuffer
     * 
     * Ritorna true se l'allocazione è andata bene se no false.
     */
    bool _allocate(size__t size){
        if (check() && size == _size)   // controllo se si sta cercando di modificare l'allocazione ma con gli stessi valori
        {
            return true;
        }

        if (size == 0){return false;}

        _buffer = new data_type[size] {};   // Le {} inizializzano tutti i valori a 0

        if (_buffer == nullptr){return false;}

        _size = size;
        
        reset();

        return true;
        
    };
public:
    RingBuffer(size_t size) {allocate(size)};
    ~RingBuffer() {delete[] _buffer};

    const data_type &pop_first() { return _buffer[_tail]; }    // Il primo const indica che il valore di ritorno può essere letto ma non modificato, mentre il secondo const dice che quel metodo non può modificare attributi della classe.
    const data_type &pop_last() { return _buffer[_head]; }

    uint32_t get_head_idx(){ return _head;}
    uint32_t get_tail_idx(){ return _tail;}

    void push(const data_type &data){
        _head = (_head + 1) % _size;

        _buffer[_head] = data;

        // Se la testa arriva alla coda allora sposto la coda avanti
        if (_head == _tail && !_first_write)
        {
            _tail = (_tail + 1) % _size;
        }

        _first_write = false;
        
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
            _first_write = true;
        }
        
    }

    data_type
};


