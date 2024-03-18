#ifndef PLUGIN_H
#define PLUGIN_H

#include <stdexcept>
#include <string>
#include <dlfcn.h>

#define __PLUGIN_MAIN __plugin_main
#define __STR_(x) #x
#define __STR(x) __STR_(x)

#define PLUGIN_MAIN(fn)\
    extern "C" int __PLUGIN_MAIN(void *state) { return fn(state); }

namespace plugin {

    template<typename T>
    class Plugin
    {
    public:
        Plugin(std::string path, T *state) : _state(state),
            _libhandle(nullptr)
        {
            _libhandle = dlopen(path.c_str(), RTLD_LAZY);
            if (!_libhandle) {
                throw std::runtime_error("Failed to load " + path);
            }

            _libmain = reinterpret_cast<int (*)(void*)>(
                dlsym(_libhandle, __STR(__PLUGIN_MAIN))
            );

            if (!_libmain) {
                throw std::runtime_error("Failed to load " __STR(__PLUGIN_MAIN));
            }
        }

        ~Plugin()
        {
            if (_libhandle)
            {
                dlclose(_libhandle);
            }
        };

        int main()
        {
            return _libmain(_state);
        }

    private:
        T *_state;
        void *_libhandle;
        int (*_libmain)(void*);
    };
}

#endif
