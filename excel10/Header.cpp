#include "stdafx.h"
#include "Header.h"
void dump_com_error(_com_error &e)
{
	_tprintf(_T("Oops - hit an error!\n"));
	_tprintf(_T("\a\tCode = %08lx\n"), e.Error());
	_tprintf(_T("\a\tCode meaning = %s\n"), e.ErrorMessage());
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	_tprintf(_T("\a\tSource = %s\n"), (LPCTSTR) bstrSource);
	_tprintf(_T("\a\tDescription = %s\n"), (LPCTSTR) bstrDescription);
}


// If this is placed in the scope of the smart pointers, they must be
// explicitly Release(d) before CoUninitialize() is called.  If any reference
// count is non-zero, a protection fault will occur.

struct StartOle _inst_StartOle;

#ifndef EXCEL
void excel()
{
	using namespace Excel;
	_ApplicationPtr pXL;

	try {
		pXL.CreateInstance(L"Excel.Application");

		pXL->Visible[0] = VARIANT_TRUE;

		WorkbooksPtr pBooks = pXL->Workbooks;
		_WorkbookPtr pBook  = pBooks->Add((long)xlWorksheet);
		_WorksheetPtr pSheet = pXL->ActiveSheet;

		//try {
		//    // This one will fail; it is done on purpose to demonstrate the error.
		//    pSheet->Name = "Market Share?";
		//} catch (_com_error &e) {
		//    dump_com_error(e);
		//}

		pSheet->Name = "Market Share!";

		// When using parameterized properties, optional args must be explicitly dealt with.
		pSheet->Range["A2"][vtMissing]->Value2 = "Company A";
		pSheet->Range["B2"][vtMissing]->Value2 = "Company B";
		pSheet->Range["C2"][vtMissing]->Value2 = "Company C";
		pSheet->Range["D2"][vtMissing]->Value2 = "Company D";

		// Of course, you can call a parameterized property as a method and then optional args are implicit.
		pSheet->GetRange("A3")->Value2 = 75.0;
		pSheet->GetRange("B3")->Value2 = 14.0;
		pSheet->GetRange("C3")->Value2 = 7.0;
		pSheet->GetRange("D3")->Value2 = 4.0;

		_WorkbookPtr pBook1  = pBooks->Add((long)xlWorksheet);
		_WorksheetPtr pSheet1 = pXL->ActiveSheet;
		pSheet1->Name = "Market Share2!";

		Sleep(1000);

		RangePtr pRange = pSheet->Range["A2:D3"][vtMissing];
		_ChartPtr pChart = pBook->Charts->Add();

		pChart->ChartWizard((Range*) pRange, (long) xl3DPie, 7L, (long) xlRows,
			1L, 0L, 2L, "Market Share");

		Sleep(6000);

		pBook->Saved[0] = VARIANT_TRUE;
		pSheet->SaveAs("C:\\01.xls");
		pXL->Quit();
	} catch(_com_error &e) {
		dump_com_error(e);
	}

}
#endif