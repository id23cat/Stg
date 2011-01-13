#pragma once
#ifndef FORM1_H
#include "stdafx.h"
#include "Header.h"
#include <JpegStegoAnalysis.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace formexcel {

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  Run;
	private: System::Windows::Forms::Button^  Source;
	private: System::Windows::Forms::Button^  Stego;
	private: System::Windows::Forms::TextBox^  SourceFileName;
	private: System::Windows::Forms::TextBox^  StegoFileName;


	private: System::Windows::Forms::GroupBox^  groupBox1;
	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Run = (gcnew System::Windows::Forms::Button());
			this->Source = (gcnew System::Windows::Forms::Button());
			this->Stego = (gcnew System::Windows::Forms::Button());
			this->SourceFileName = (gcnew System::Windows::Forms::TextBox());
			this->StegoFileName = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->SuspendLayout();
			// 
			// Run
			// 
			this->Run->Location = System::Drawing::Point(249, 207);
			this->Run->Name = L"Run";
			this->Run->Size = System::Drawing::Size(75, 23);
			this->Run->TabIndex = 0;
			this->Run->Text = L"Run";
			this->Run->UseVisualStyleBackColor = true;
			this->Run->Click += gcnew System::EventHandler(this, &Form1::Run_Click);
			// 
			// Source
			// 
			this->Source->Location = System::Drawing::Point(249, 26);
			this->Source->Name = L"Source";
			this->Source->Size = System::Drawing::Size(75, 23);
			this->Source->TabIndex = 1;
			this->Source->Text = L"Исходный";
			this->Source->UseVisualStyleBackColor = true;
			this->Source->Click += gcnew System::EventHandler(this, &Form1::Source_Click);
			// 
			// Stego
			// 
			this->Stego->Location = System::Drawing::Point(249, 73);
			this->Stego->Name = L"Stego";
			this->Stego->Size = System::Drawing::Size(75, 23);
			this->Stego->TabIndex = 1;
			this->Stego->Text = L"Стего";
			this->Stego->UseVisualStyleBackColor = true;
			this->Stego->Click += gcnew System::EventHandler(this, &Form1::Stego_Click);
			// 
			// SourceFileName
			// 
			this->SourceFileName->Location = System::Drawing::Point(58, 28);
			this->SourceFileName->Name = L"SourceFileName";
			this->SourceFileName->Size = System::Drawing::Size(146, 20);
			this->SourceFileName->TabIndex = 2;
			// 
			// StegoFileName
			// 
			this->StegoFileName->Location = System::Drawing::Point(58, 76);
			this->StegoFileName->Name = L"StegoFileName";
			this->StegoFileName->Size = System::Drawing::Size(146, 20);
			this->StegoFileName->TabIndex = 2;
			// 
			// groupBox1
			// 
			this->groupBox1->Location = System::Drawing::Point(47, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(290, 118);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Выберите файлы";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(363, 262);
			this->Controls->Add(this->StegoFileName);
			this->Controls->Add(this->SourceFileName);
			this->Controls->Add(this->Stego);
			this->Controls->Add(this->Source);
			this->Controls->Add(this->Run);
			this->Controls->Add(this->groupBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Run_Click(System::Object^  sender, System::EventArgs^  e) {
				 JpegStegoAnalysis jsa;
				 excel();

			 }
	private: System::Void Source_Click(System::Object^  sender, System::EventArgs^  e) {
				
				OpenFileDialog fileDlg;
				fileDlg.InitialDirectory = "c:\\";
				fileDlg.Filter = "Bitmap files (*.bmp)|*.bmp";
				fileDlg.FilterIndex = 1;
				fileDlg.RestoreDirectory = false;
				if ( fileDlg.ShowDialog() == ::DialogResult::OK)
				{
					SourceFileName->Text=fileDlg.FileName;
				}


			 }
private: System::Void Stego_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenFileDialog fileDlg;
			 fileDlg.InitialDirectory = "c:\\";
			 fileDlg.Filter = "Bitmap files (*.bmp)|*.bmp";
			 fileDlg.FilterIndex = 1;
			 fileDlg.RestoreDirectory = false;
			 if ( fileDlg.ShowDialog() == ::DialogResult::OK)
				{
					StegoFileName->Text=fileDlg.FileName;					
				}
		 }
};
}

#endif