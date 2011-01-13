#pragma once

#include <stdio.h>
#include "libstego.h"
#include <vcclr.h>

namespace WindowStego {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

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
	private: System::Windows::Forms::ComboBox^  comboType;
	protected: 

	private: System::Windows::Forms::RadioButton^  radioHide;
	private: System::Windows::Forms::RadioButton^  radioExtract;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::ListBox^  listSrc;

	private: System::Windows::Forms::Button^  AddSrcBtn;
	private: System::Windows::Forms::Button^  dstFolderBtn;

	private: System::Windows::Forms::Button^  remSrcBtn;
	private: System::Windows::Forms::Button^  MessageBtn;

	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::TextBox^  MessageTbox;
	private: System::Windows::Forms::Button^  startBtn;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  statusLabel;
	private: System::Windows::Forms::TextBox^  dstFolderTbox;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  capacityLabel;
	private: System::Windows::Forms::Label^  label3;
	



	//protected: 

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
		void InitializeComponent(void){
			this->comboType = (gcnew System::Windows::Forms::ComboBox());
			this->radioHide = (gcnew System::Windows::Forms::RadioButton());
			this->radioExtract = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->capacityLabel = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->remSrcBtn = (gcnew System::Windows::Forms::Button());
			this->AddSrcBtn = (gcnew System::Windows::Forms::Button());
			this->listSrc = (gcnew System::Windows::Forms::ListBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->dstFolderTbox = (gcnew System::Windows::Forms::TextBox());
			this->dstFolderBtn = (gcnew System::Windows::Forms::Button());
			this->MessageBtn = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->MessageTbox = (gcnew System::Windows::Forms::TextBox());
			this->startBtn = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->statusLabel = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// comboType
			// 
			this->comboType->FormattingEnabled = true;
			this->comboType->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"BMP files", L"JPEG files", L"Ogg files"});
			this->comboType->Location = System::Drawing::Point(18, 32);
			this->comboType->Name = L"comboType";
			this->comboType->Size = System::Drawing::Size(121, 21);
			this->comboType->TabIndex = 0;
			this->comboType->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboType_SelectedIndexChanged);
			// 
			// radioHide
			// 
			this->radioHide->AutoSize = true;
			this->radioHide->Checked = true;
			this->radioHide->Location = System::Drawing::Point(174, 13);
			this->radioHide->Name = L"radioHide";
			this->radioHide->Size = System::Drawing::Size(93, 17);
			this->radioHide->TabIndex = 1;
			this->radioHide->TabStop = true;
			this->radioHide->Text = L"Hide Message";
			this->radioHide->UseVisualStyleBackColor = true;
			this->radioHide->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioHide_CheckedChanged);
			// 
			// radioExtract
			// 
			this->radioExtract->AutoSize = true;
			this->radioExtract->Location = System::Drawing::Point(174, 36);
			this->radioExtract->Name = L"radioExtract";
			this->radioExtract->Size = System::Drawing::Size(104, 17);
			this->radioExtract->TabIndex = 2;
			this->radioExtract->TabStop = true;
			this->radioExtract->Text = L"Extract Message";
			this->radioExtract->UseVisualStyleBackColor = true;
			this->radioExtract->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioExtract_CheckedChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button2);
			this->groupBox1->Controls->Add(this->capacityLabel);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->remSrcBtn);
			this->groupBox1->Controls->Add(this->AddSrcBtn);
			this->groupBox1->Controls->Add(this->listSrc);
			this->groupBox1->Location = System::Drawing::Point(13, 61);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(288, 155);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Source Files";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(235, 68);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(47, 23);
			this->button2->TabIndex = 5;
			this->button2->Text = L"Down";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// capacityLabel
			// 
			this->capacityLabel->AutoSize = true;
			this->capacityLabel->Location = System::Drawing::Point(214, 128);
			this->capacityLabel->Name = L"capacityLabel";
			this->capacityLabel->Size = System::Drawing::Size(13, 13);
			this->capacityLabel->TabIndex = 7;
			this->capacityLabel->Text = L"0";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(139, 128);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(80, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Total capacity: ";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(235, 39);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(47, 23);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Up";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// remSrcBtn
			// 
			this->remSrcBtn->Location = System::Drawing::Point(72, 123);
			this->remSrcBtn->Name = L"remSrcBtn";
			this->remSrcBtn->Size = System::Drawing::Size(61, 23);
			this->remSrcBtn->TabIndex = 4;
			this->remSrcBtn->Text = L"Remove";
			this->remSrcBtn->UseVisualStyleBackColor = true;
			this->remSrcBtn->Click += gcnew System::EventHandler(this, &Form1::remSrcBtn_Click);
			// 
			// AddSrcBtn
			// 
			this->AddSrcBtn->Location = System::Drawing::Point(6, 123);
			this->AddSrcBtn->Name = L"AddSrcBtn";
			this->AddSrcBtn->Size = System::Drawing::Size(59, 23);
			this->AddSrcBtn->TabIndex = 3;
			this->AddSrcBtn->Text = L"Add";
			this->AddSrcBtn->UseVisualStyleBackColor = true;
			this->AddSrcBtn->Click += gcnew System::EventHandler(this, &Form1::AddSrcBtn_Click);
			// 
			// listSrc
			// 
			this->listSrc->FormattingEnabled = true;
			this->listSrc->HorizontalScrollbar = true;
			this->listSrc->Location = System::Drawing::Point(9, 20);
			this->listSrc->Name = L"listSrc";
			this->listSrc->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
			this->listSrc->Size = System::Drawing::Size(220, 95);
			this->listSrc->TabIndex = 0;
			this->listSrc->TabStop = false;
			this->listSrc->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listSrc_SelectedIndexChanged);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->dstFolderTbox);
			this->groupBox2->Controls->Add(this->dstFolderBtn);
			this->groupBox2->Location = System::Drawing::Point(13, 224);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(288, 54);
			this->groupBox2->TabIndex = 0;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Destination Folder";
			// 
			// dstFolderTbox
			// 
			this->dstFolderTbox->Location = System::Drawing::Point(91, 19);
			this->dstFolderTbox->Name = L"dstFolderTbox";
			this->dstFolderTbox->Size = System::Drawing::Size(186, 20);
			this->dstFolderTbox->TabIndex = 6;
			// 
			// dstFolderBtn
			// 
			this->dstFolderBtn->Location = System::Drawing::Point(6, 19);
			this->dstFolderBtn->Name = L"dstFolderBtn";
			this->dstFolderBtn->Size = System::Drawing::Size(75, 23);
			this->dstFolderBtn->TabIndex = 5;
			this->dstFolderBtn->Text = L"Select";
			this->dstFolderBtn->UseVisualStyleBackColor = true;
			this->dstFolderBtn->Click += gcnew System::EventHandler(this, &Form1::dstFolder_Click);
			// 
			// MessageBtn
			// 
			this->MessageBtn->Location = System::Drawing::Point(6, 19);
			this->MessageBtn->Name = L"MessageBtn";
			this->MessageBtn->Size = System::Drawing::Size(75, 23);
			this->MessageBtn->TabIndex = 6;
			this->MessageBtn->Text = L"Message";
			this->MessageBtn->UseVisualStyleBackColor = true;
			this->MessageBtn->Click += gcnew System::EventHandler(this, &Form1::MessageBtn_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->MessageTbox);
			this->groupBox3->Controls->Add(this->MessageBtn);
			this->groupBox3->Location = System::Drawing::Point(13, 287);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(288, 50);
			this->groupBox3->TabIndex = 4;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Message File";
			// 
			// MessageTbox
			// 
			this->MessageTbox->Location = System::Drawing::Point(91, 20);
			this->MessageTbox->Name = L"MessageTbox";
			this->MessageTbox->Size = System::Drawing::Size(186, 20);
			this->MessageTbox->TabIndex = 15;
			// 
			// startBtn
			// 
			this->startBtn->Location = System::Drawing::Point(102, 350);
			this->startBtn->Name = L"startBtn";
			this->startBtn->Size = System::Drawing::Size(107, 23);
			this->startBtn->TabIndex = 5;
			this->startBtn->Text = L"START";
			this->startBtn->UseVisualStyleBackColor = true;
			this->startBtn->Click += gcnew System::EventHandler(this, &Form1::startBtn_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(19, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(111, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Select Container type:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 380);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Status: ";
			// 
			// statusLabel
			// 
			this->statusLabel->AutoSize = true;
			this->statusLabel->Location = System::Drawing::Point(55, 380);
			this->statusLabel->Name = L"statusLabel";
			this->statusLabel->Size = System::Drawing::Size(130, 13);
			this->statusLabel->TabIndex = 8;
			this->statusLabel->Text = L"Chose properties for stego";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(314, 397);
			this->Controls->Add(this->statusLabel);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->startBtn);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->radioExtract);
			this->Controls->Add(this->radioHide);
			this->Controls->Add(this->comboType);
			this->Controls->Add(this->groupBox3);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"Form1";
			this->Text = L"WindowStego";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void comboType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 //statusLabel->Text="Chose Hide or Extract data";
			 }
private: System::Void radioHide_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void radioExtract_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void AddSrcBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenFileDialog^ fDial = gcnew OpenFileDialog;
			 fDial->InitialDirectory=".\\";
			 fDial->Multiselect = true;
			 //fDial->RestoreDirectory = false;
			 int index = comboType->SelectedIndex;
			 switch(index)
			 {
			 case 0:
				 fDial->Filter = "Bitmap files (*.bmp)|*.bmp";
				 break;
			 case 1:
				 fDial->Filter = "JPEG files (*.jpg)|*.jpg";
				 break;
			 case 2:
				 fDial->Filter = "Audio files (*.ogg, *.wav)|*.ogg; *.wav";
				 break;
			 default: 
				 statusLabel->Text="Select container type";
				 return;
			 }
			  statusLabel->Text="Chose properties for stego";
			 //fDial->FilterIndex = 1;
			 fDial->RestoreDirectory=false;

			 if( fDial->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 listSrc->BeginUpdate();
				 array<String^>^ files = fDial->FileNames;
				 /*int length = files->get_Length();*/
				 
				 for(int i=0;i<files->Length;i++)
				 /*while(String file = files->ForEach()[i++])*/
				 {
					 listSrc->Items->Add(files[i]);
				 }
				 
				 listSrc->EndUpdate();
			 }
			 if(radioHide->Checked)
			 {
				CalculateCapacity();
				statusLabel->Text = "Calculate done!";
				}
			
			 
			 
			 //statusLabel->Text="Select containers";

		 }
private: System::Void listSrc_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void remSrcBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			 listSrc->BeginUpdate();
			 for(int i=0;i<listSrc->SelectedItems->Count;i++)
				 listSrc->Items->Remove(listSrc->Items[i]);
			/* listSrc->Items->Remove(listSrc->SelectedItem);*/
			if(radioHide->Checked)	
			 if(!CalculateCapacity())
				statusLabel->Text = "Calculate done!";
			 listSrc->EndUpdate();
		 }
private: System::Void dstFolder_Click(System::Object^  sender, System::EventArgs^  e) {
			 FolderBrowserDialog^  folderDial = gcnew FolderBrowserDialog;

			 if (folderDial->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			 {
				 dstFolderTbox->Text = folderDial->SelectedPath;
			 }
		 }
private: System::Void MessageBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(radioHide->Checked)
			 {
				 OpenFileDialog^ fDial = gcnew OpenFileDialog;
				 //fDial->InitialDirectory=".";
				 fDial->Multiselect = true;
				 fDial->RestoreDirectory = false;
				 fDial->Filter = "All files (*.*)|*.*";
				 if( fDial->ShowDialog()==System::Windows::Forms::DialogResult::OK)
				 {
					 MessageTbox->Text = fDial->FileName;
				 }
			 }
			 else if(radioExtract->Checked)
			 {
				 SaveFileDialog^ fDial = gcnew SaveFileDialog;
				 fDial->RestoreDirectory = false;
				 fDial->Filter = "All files (*.*)|*.*";
				 if( fDial->ShowDialog()==System::Windows::Forms::DialogResult::OK)
				 {
					 MessageTbox->Text = fDial->FileName;
				 }
			 }
		 }
private: System::Void startBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			 statusLabel->Text = "In process...";
			 this->Update();
			 int index = comboType->SelectedIndex;
			 //Thread^ oThread;
			 switch(index)
			 {
			 case 0:
				 if(radioHide->Checked)
				 {
					 /*oThread = gcnew Thread(gcnew ParameterizedThreadStart(&Form1::EncodeBmp));
					 oThread->Start(this);*/
					 EncodeBmp(this);
				 }else if(radioExtract->Checked)
				 {
					 /*oThread = gcnew Thread(gcnew ParameterizedThreadStart(&Form1::DecodeBmp));
					 oThread->Start(this);*/
					  DecodeBmp(this);
				 }
				 break;
			 case 1:
				 if(radioHide->Checked)
				 {
					/* oThread = gcnew Thread(gcnew ParameterizedThreadStart(&Form1::EncodeJpeg));
					 oThread->Start(this);*/
					 EncodeJpeg(this);
				 }else if(radioExtract->Checked)
				 {
					 /*oThread = gcnew Thread(gcnew ParameterizedThreadStart(&Form1::DecodeJpeg));
					 oThread->Start(this);*/
					 DecodeJpeg(this);
				 }
				 
				 break;
			 case 2:
				 if(radioHide->Checked)
				 {
					/* oThread = gcnew Thread(gcnew ParameterizedThreadStart(&Form1::EncodeOgg));
					 oThread->Start(this);*/
					 EncodeOgg(this);

				 }else if(radioExtract->Checked)
				 {
					 /*oThread = gcnew Thread(gcnew ParameterizedThreadStart(&Form1::DecodeOgg));
					 oThread->Start(this);*/
					 DecodeOgg(this);
				 }		
					
				 break;
			 default: 
				
				 return;
			 }
			 statusLabel->Text = "All done!";
		 }
public: static void EncodeJpeg(Object^ o){
			Form1^ ob = dynamic_cast<Form1^>(o);
			try{
			try
			{
				JpegStegoEncoder jse;
				char *mesfile = GetCharPtr(ob->MessageTbox->Text);
				jse.SetMessageFile(mesfile);				
				//jse.LOG(ob->checkLog->Checked);
				for(int i=0;i<ob->listSrc->Items->Count;i++)
				{
					String^ src = dynamic_cast<String^>(ob->listSrc->Items[i]);
					String^ dst = AplyPath(src, ob->dstFolderTbox->Text);					
					char *srcch = GetCharPtr(src);
					char *dstch = GetCharPtr(dst);
					
					jse.Encode(srcch,dstch,true);
				}
				
			}			
			catch (::Exception e)
			{
				ob->statusLabel->Text = "Error: ";
				ob->statusLabel->Text = ob->statusLabel->Text + gcnew String(e.getMessage());
				//return false;
			}			
			ob->statusLabel->Text = "All done!";
			}catch(...){}

			//return true;			
		}

private: static void DecodeJpeg(Object^ o){
			 Form1^ ob = dynamic_cast<Form1^>(o);
			 try{
			 try
			 {
				 JpegStegoDecoder jsd;
				 for(int i=0;i<ob->listSrc->Items->Count;i++)
				 {
					 String^ src = dynamic_cast<String^>(ob->listSrc->Items[i]);
					 
					 char *srcch = GetCharPtr(src);					 					 
					 jsd.Decode(srcch);					
				 }
				 char *mesfile = GetCharPtr(ob->MessageTbox->Text);
				 jsd.SaveMessageToFile(mesfile);

			 }			
			 catch (::Exception e)
			 {
				 ob->statusLabel->Text = "Error: ";
				 ob->statusLabel->Text = ob->statusLabel->Text + gcnew String(e.getMessage());
				 //return false;
			 }			
			 ob->statusLabel->Text = "All done!";
			 }catch(...){}

			 //return true;			
		 }

private: static void EncodeOgg(Object^ o){
			 Form1^ ob = dynamic_cast<Form1^>(o);
			try{
			 try
			 {
				 OggStegoEncoder ose;
				 char *mesfile = GetCharPtr(ob->MessageTbox->Text);
				 ose.SetMessageFile(mesfile);
				 for(int i=0;i<ob->listSrc->Items->Count;i++)
				 {
					 String^ src = dynamic_cast<String^>(ob->listSrc->Items[i]);
					 String^ dst = AplyPath(src, ob->dstFolderTbox->Text);

					 char *srcch = GetCharPtr(src);
					 char *dstch = GetCharPtr(dst);					 
					 ose.Encode(srcch,dstch,true);
				 }

			 }			
			 catch (::Exception e)
			 {
				 ob->statusLabel->Text = "Error: ";
				 ob->statusLabel->Text = ob->statusLabel->Text + gcnew String(e.getMessage());
				 //return false;
			 }			
			 ob->statusLabel->Text = "All done!";
			 }catch(...){}

			 //return true;			
		 }

private: static void DecodeOgg(Object^ o){
			 Form1^ ob = dynamic_cast<Form1^>(o);
			 try{
			 try
			 {
				 OggStegoDecoder jsd;
				 for(int i=0;i<ob->listSrc->Items->Count;i++)
				 {
					 String^ src = dynamic_cast<String^>(ob->listSrc->Items[i]);

					 char *srcch = GetCharPtr(src);					 
					
					 jsd.Decode(srcch);
					 
				 }
				  char *mesfile = GetCharPtr(ob->MessageTbox->Text);
				  jsd.SaveMessageToFile(mesfile);

			 }			
			 catch (::Exception e)
			 {
				 ob->statusLabel->Text = "Error: ";
				 ob->statusLabel->Text = ob->statusLabel->Text + gcnew String(e.getMessage());
				 //return false;
			 }	
			 
			 ob->statusLabel->Text = "All done!";
			 }catch(...){}

			 //return true;			
		 }

private: static void EncodeBmp(Object^ o){
			 Form1^ ob = dynamic_cast<Form1^>(o);
			 try{
			 try
			 {
				 LSBStegoEncoder lse;
				 char *mesfile = GetCharPtr(ob->MessageTbox->Text);
				 lse.SetMessageFile(mesfile);
				 for(int i=0;i<ob->listSrc->Items->Count;i++)
				 {
					 String^ src = dynamic_cast<String^>(ob->listSrc->Items[i]);
					 String^ dst = AplyPath(src, ob->dstFolderTbox->Text);

					 char *srcch = GetCharPtr(src);
					 char *dstch = GetCharPtr(dst);
					 
					 lse.Encode(srcch,dstch,true);
				 }

			 }			
			 catch (::Exception e)
			 {
				 ob->statusLabel->Text = "Error: ";
				 ob->statusLabel->Text = ob->statusLabel->Text + gcnew String(e.getMessage());
				 //return false;
			 }			
			 ob->statusLabel->Text = "All done!";
			}catch(...){}

			 //return true;			
		 }

private: static void DecodeBmp(Object^ o){
			 Form1^ ob = dynamic_cast<Form1^>(o);
			try{
			 try
			 {
				 LSBStegoDecoder lsd;
				 for(int i=0;i<ob->listSrc->Items->Count;i++)
				 {
					 String^ src = dynamic_cast<String^>(ob->listSrc->Items[i]);

					 char *srcch = GetCharPtr(src);					 
					 
					 lsd.Decode(srcch);
					
				 }
				 char *mesfile = GetCharPtr(ob->MessageTbox->Text);
				  lsd.SaveMessageToFile(mesfile);

			 }			
			 catch (::Exception e)
			 {
				 ob->statusLabel->Text = "Error: ";
				 ob->statusLabel->Text = ob->statusLabel->Text + gcnew String(e.getMessage());
				 //return false;
			 }			
			 ob->statusLabel->Text = "All done!";
			 
			 }catch(...){}

			 //return true;			
		 }
private: static String^ AplyPath(String^ srcFile, String^ path)
		 {
			 String^ result = path;
			 result = result +"\\" +System::IO::Path::GetFileName(srcFile);
			 return result;
		 }
private: static char* GetCharPtr(String^ str)
		 {
			 array<unsigned char>^ arr = 
				 System::Text::Encoding::GetEncoding(866)->GetBytes(str);
			 pin_ptr<unsigned char> ptr = &arr[0];
			 char *ch = (char*)ptr;
			 return ch;
		 }

public:

	int CalculateCapacity(void)
	{
		statusLabel->Text = "Calculate capacity";
		Update();
		try
		{
			int index = comboType->SelectedIndex;
			StegoEncoder *pse;
			switch(index)
			{
			case 0:
				pse = new LSBStegoEncoder;
				break;
			case 1:
				pse = new JpegStegoEncoder;
				break;
			case 2:
				pse = new OggStegoEncoder;
				break;
			default: 					 
				return 1;
			}				 
			size_t len=0;// = Convert::ToUInt32(capacityLabel->Text);
			for(int i=0;i<listSrc->Items->Count;i++)
			{
				String^ src = dynamic_cast<String^>(listSrc->Items[i]);

				char *srcch = GetCharPtr(src);
				len += pse->Test(srcch,true);
			}
			char buf[6];
			_itoa(len,buf,10);
			capacityLabel->Text = gcnew String(buf);
			this->Update();
			delete pse;
		}			
		catch (::Exception e)
		{
			statusLabel->Text = "Error: ";
			statusLabel->Text = statusLabel->Text + gcnew String(e.getMessage())+ gcnew String(" bytes");
			this->Update();
			return 1;// false;
		}			
		return 0;
	}
};


}

