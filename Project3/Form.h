#pragma once
#ifndef FORM_H
#define FORM_H

#include "MyServer.h" 

using namespace System;

public ref class Form 
{
public:
    Form() {
        server = gcnew MyServer(); 
        server->start();
    }

    ~Form() {
        this->!Form(); 
    }

protected:
    !Form() { 
        server->stop(); 
    }

   
    void OnClose() {
        this->!Form(); 
    }

private:
    MyServer^ server; 
};

#endif // FORM_H
