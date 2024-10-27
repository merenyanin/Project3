#pragma once

#include <iostream>
#include "MyServer.h"
#include "Form.h"

namespace Project2 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            myServer = gcnew MyServer(); // ≤н≥ц≥ал≥зуйте сервер тут
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
            if (myServer) {
                myServer->stop(); // «упин≥ть сервер
                delete myServer; // ¬ив≥льн≥ть пам'€ть
            }
        }

    private:
        void MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
            
        }
        System::ComponentModel::Container^ components;
        MyServer^ myServer; 
        System::Windows::Forms::Panel^ drawingPanel;
        System::Windows::Forms::Button^ clearButton;
        System::Windows::Forms::Button^ drawPixelButton;
        System::Windows::Forms::Button^ drawRectangleButton;
        System::Windows::Forms::Button^ fillRectangleButton;
        System::Windows::Forms::Button^ fillEllipseButton;
        System::Windows::Forms::Button^ drawEllipseButton;
        System::Windows::Forms::Button^ drawLineButton;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->drawingPanel = (gcnew System::Windows::Forms::Panel());
            this->clearButton = (gcnew System::Windows::Forms::Button());
            this->drawPixelButton = (gcnew System::Windows::Forms::Button());
            this->drawLineButton = (gcnew System::Windows::Forms::Button());
            this->drawRectangleButton = (gcnew System::Windows::Forms::Button());
            this->fillRectangleButton = (gcnew System::Windows::Forms::Button());
            this->fillEllipseButton = (gcnew System::Windows::Forms::Button());
            this->drawEllipseButton = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();

            // drawingPanel
            this->drawingPanel->BackColor = System::Drawing::Color::White;
            this->drawingPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->drawingPanel->Location = System::Drawing::Point(10, 10);
            this->drawingPanel->Name = L"drawingPanel";
            this->drawingPanel->Size = System::Drawing::Size(800, 500);
            this->drawingPanel->TabIndex = 0;

            // clearButton
            this->clearButton->Location = System::Drawing::Point(820, 30);
            this->clearButton->Name = L"clearButton";
            this->clearButton->Size = System::Drawing::Size(150, 30);
            this->clearButton->TabIndex = 1;
            this->clearButton->Text = L"Clear Display";
            this->clearButton->Click += gcnew System::EventHandler(this, &MyForm::clearButton_Click);

            // drawPixelButton
            this->drawPixelButton->Location = System::Drawing::Point(820, 70);
            this->drawPixelButton->Name = L"drawPixelButton";
            this->drawPixelButton->Size = System::Drawing::Size(150, 30);
            this->drawPixelButton->TabIndex = 2;
            this->drawPixelButton->Text = L"Draw Pixel";
            this->drawPixelButton->Click += gcnew System::EventHandler(this, &MyForm::drawPixelButton_Click);

            // drawLineButton
            this->drawLineButton->Location = System::Drawing::Point(820, 110);
            this->drawLineButton->Name = L"drawLineButton";
            this->drawLineButton->Size = System::Drawing::Size(150, 30);
            this->drawLineButton->TabIndex = 3;
            this->drawLineButton->Text = L"Draw Line";
            this->drawLineButton->Click += gcnew System::EventHandler(this, &MyForm::drawLineButton_Click);

            // drawRectangleButton
            this->drawRectangleButton->Location = System::Drawing::Point(820, 155);
            this->drawRectangleButton->Name = L"drawRectangleButton";
            this->drawRectangleButton->Size = System::Drawing::Size(150, 30);
            this->drawRectangleButton->TabIndex = 4;
            this->drawRectangleButton->Text = L"Draw Rectangle";
            this->drawRectangleButton->Click += gcnew System::EventHandler(this, &MyForm::drawRectangleButton_Click);

            // fillRectangleButton
            this->fillRectangleButton->Location = System::Drawing::Point(820, 201);
            this->fillRectangleButton->Name = L"fillRectangleButton";
            this->fillRectangleButton->Size = System::Drawing::Size(150, 30);
            this->fillRectangleButton->TabIndex = 5;
            this->fillRectangleButton->Text = L"Fill Rectangle";
            this->fillRectangleButton->Click += gcnew System::EventHandler(this, &MyForm::fillRectangleButton_Click);

            // fillEllipseButton
            this->fillEllipseButton->Location = System::Drawing::Point(820, 247);
            this->fillEllipseButton->Name = L"fillEllipseButton";
            this->fillEllipseButton->Size = System::Drawing::Size(150, 30);
            this->fillEllipseButton->TabIndex = 6;
            this->fillEllipseButton->Text = L"Fill Ellipse";
            this->fillEllipseButton->Click += gcnew System::EventHandler(this, &MyForm::fillEllipseButton_Click);

            // drawEllipseButton
            this->drawEllipseButton->Location = System::Drawing::Point(820, 298);
            this->drawEllipseButton->Name = L"drawEllipseButton";
            this->drawEllipseButton->Size = System::Drawing::Size(150, 30);
            this->drawEllipseButton->TabIndex = 7;
            this->drawEllipseButton->Text = L"Draw Ellipse";
            this->drawEllipseButton->Click += gcnew System::EventHandler(this, &MyForm::drawEllipseButton_Click);

            // MyForm
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1016, 520);
            this->Controls->Add(this->fillEllipseButton);
            this->Controls->Add(this->fillRectangleButton);
            this->Controls->Add(this->drawEllipseButton);
            this->Controls->Add(this->drawRectangleButton);
            this->Controls->Add(this->drawingPanel);
            this->Controls->Add(this->clearButton);
            this->Controls->Add(this->drawPixelButton);
            this->Controls->Add(this->drawLineButton);
            this->Name = L"MyForm";
            this->Text = L"MyForm";
            this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
            this->ResumeLayout(false);
        }
#pragma endregion

    private:
        System::Void clearButton_Click(System::Object^ sender, System::EventArgs^ e) {
            // Clear display logic
            Graphics^ g = drawingPanel->CreateGraphics();
            g->Clear(System::Drawing::Color::White);

            // Create command and send to server
            ClearDisplay^ clearCommand = gcnew ClearDisplay(0); 
            myServer->sendCommand(clearCommand);
        }

        System::Void drawPixelButton_Click(System::Object^ sender, System::EventArgs^ e) {
            Graphics^ g = drawingPanel->CreateGraphics();
            Random^ rand = gcnew Random();
            int x = rand->Next(0, drawingPanel->Width - 5);
            int y = rand->Next(0, drawingPanel->Height - 5);
            g->FillRectangle(gcnew SolidBrush(System::Drawing::Color::Red), x, y, 5, 5);

            // Create command and send to server
            DrawPixel^ pixelCommand = gcnew DrawPixel(x, y, 0xFF0000); // Red color
            myServer->sendCommand(pixelCommand);
        }

        System::Void drawLineButton_Click(System::Object^ sender, System::EventArgs^ e) {
            Graphics^ g = drawingPanel->CreateGraphics();
            g->DrawLine(gcnew Pen(System::Drawing::Color::Blue), 150, 150, 400, 400);

            // Create command and send to server
            DrawLine^ lineCommand = gcnew DrawLine(150, 150, 400, 400, 0x0000FF); // Blue color
            myServer->sendCommand(lineCommand);
        }

        System::Void drawRectangleButton_Click(System::Object^ sender, System::EventArgs^ e) {
            Graphics^ g = drawingPanel->CreateGraphics();
            g->DrawRectangle(gcnew Pen(System::Drawing::Color::Green), 200, 100, 150, 80);

            // Create command and send to server
            DrawRectangle^ rectCommand = gcnew DrawRectangle(200, 100, 150, 80, 0x00FF00); // Green color
            myServer->sendCommand(rectCommand);
        }

        System::Void fillRectangleButton_Click(System::Object^ sender, System::EventArgs^ e) {
            Graphics^ g = drawingPanel->CreateGraphics();
            g->FillRectangle(gcnew SolidBrush(System::Drawing::Color::Yellow), 200, 100, 150, 80);

            // Create command and send to server
            FillRectangle^ fillRectCommand = gcnew FillRectangle(200, 100, 150, 80, 0xFFFF00); // Yellow color
            myServer->sendCommand(fillRectCommand);
        }

        System::Void fillEllipseButton_Click(System::Object^ sender, System::EventArgs^ e) {
            Graphics^ g = drawingPanel->CreateGraphics();
            g->FillEllipse(gcnew SolidBrush(System::Drawing::Color::Pink), 100, 200, 150, 100);

            // Create command and send to server
            FillEllipse^ fillEllipseCommand = gcnew FillEllipse(100, 200, 150, 100, 0xFFC0CB); // Pink color
            myServer->sendCommand(fillEllipseCommand);
        }

        System::Void drawEllipseButton_Click(System::Object^ sender, System::EventArgs^ e) {
            Graphics^ g = drawingPanel->CreateGraphics();
            g->DrawEllipse(gcnew Pen(System::Drawing::Color::Orange), 100, 200, 150, 100);

            // Create command and send to server
            DrawEllipse^ ellipseCommand = gcnew DrawEllipse(100, 200, 150, 100, 0xFFA500); // Orange color
            myServer->sendCommand(ellipseCommand);
        }
    };
}
