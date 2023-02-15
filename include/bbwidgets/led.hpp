#pragma once

#ifdef SHARED_LIBRARY
#define DLL_EXPORT    __declspec(dllexport)
#else
#define DLL_EXPORT    __declspec(dllimport)
#endif

#include <QWidget>

namespace bbwidgets {


    class DLL_EXPORT Led : public QWidget {
    public:

    private:

    };


}
