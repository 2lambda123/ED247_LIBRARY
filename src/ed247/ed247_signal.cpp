/******************************************************************************
 * The MIT Licence
 *
 * Copyright (c) 2021 Airbus Operations S.A.S
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/

#include "ed247_signal.h"
#include "ed247_stream.h"

#include <regex>

namespace ed247
{

// SignalBuilder<>

template<ed247_signal_type_t ... E>
signal_ptr_t SignalBuilder<E...>::create(const ed247_signal_type_t & type, const xml::Signal* configuration, BaseStream & stream)
{
    THROW_ED247_ERROR("Cannot create signal [" << std::string(configuration->_name) << "]");
    return nullptr;
}

template<ed247_signal_type_t T, ed247_signal_type_t ... E>
signal_ptr_t SignalBuilder<T, E...>::create(const ed247_signal_type_t & type, const xml::Signal* configuration, BaseStream & stream)
{
    if(type == T){
        static typename Signal<T>::Builder builder;
        return builder.create(configuration, stream);
    }else{
        return SignalBuilder<E...>::create(type, configuration, stream);
    }
}

template<ed247_signal_type_t T>
signal_ptr_t SignalBuilder<T>::create(const ed247_signal_type_t & type, const xml::Signal* configuration, BaseStream & stream)
{
    if(type == T){
        static typename Signal<T>::Builder builder;
        return builder.create(configuration, stream);
    }else{
        THROW_ED247_ERROR("Failed to create singal '" << configuration->_name << "': type mismatch");
    }
}

// BaseSignal

std::unique_ptr<BaseSample> BaseSignal::allocate_sample() const
{
    auto sample = std::make_unique<BaseSample>();
    sample->allocate(get_sample_max_size_bytes());
    return sample;
}

// BaseSignal::Pool

signal_ptr_t BaseSignal::Pool::get(const xml::Signal* configuration, BaseStream & stream)
{
    signal_ptr_t sp_base_signal;
    std::string name{configuration->_name};
    auto sp_stream = stream.shared_from_this();

    auto iter = std::find_if(_signals.begin(), _signals.end(),
        [&name, &sp_stream](const signal_ptr_t & s){ return s->get_name() == name && s->_stream.lock() == sp_stream; });
    if(iter != _signals.end())
        THROW_ED247_ERROR("Signal [" << (*iter)->get_name() << "] linked to stream [" << stream.get_name() << "] already exist !");
    auto sp_signal = _builder.create(configuration->_type, configuration, stream);
    sp_base_signal = std::static_pointer_cast<BaseSignal>(sp_signal);
    _signals.push_back(sp_base_signal);

    return sp_base_signal;
}

signal_list_t BaseSignal::Pool::find(std::string str_regex)
{
    std::regex reg(str_regex);
    signal_list_t founds;
    for(auto signal : _signals){
        if(std::regex_match(signal->get_name(), reg)){
            founds.push_back(signal);
        }
    }
    return founds;
}

signal_ptr_t BaseSignal::Pool::get(std::string str_name)
{
    for(auto signal : _signals){
        if(signal->get_name() == str_name) return signal;
    }
    return nullptr;
}

uint32_t BaseSignal::Pool::size() const
{
    return _signals.size();
}

// BaseSignal::Builder

signal_ptr_t BaseSignal::Builder::build(std::shared_ptr<Pool> & pool, const xml::Signal* configuration, BaseStream & stream) const
{
    return pool->get(configuration, stream);
}

// Signal<>::Builder

template<ed247_signal_type_t E>
std::shared_ptr<Signal<E>> Signal<E>::Builder::create(const xml::Signal* configuration, BaseStream & stream) const
{
    auto sp_stream = stream.shared_from_this();
    return std::make_shared<Signal<E>>(configuration, sp_stream);
}


}
