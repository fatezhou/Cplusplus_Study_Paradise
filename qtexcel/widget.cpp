#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

}

void Widget::writeExl()
{
    //QString filepath=QFileDialog::getSaveFileName(this,tr("Save orbit"),".",tr("Microsoft Office 2007 (*.xls)"));//获取保存路径
    QString filepath= "c:/89.xls";//获取保存路径
        if(!filepath.isEmpty()){
            QAxObject *excel = new QAxObject(this);
            excel->setControl("Excel.Application");//连接Excel控件
            excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示

            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建一个工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *worksheets = workbook->querySubObject("Sheets");//获取工作表集合
            QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);//获取工作表集合的工作表1，即sheet1
            QAxObject *cellX,*cellY;
            for(int i=0;i<9;i++){
                QString X="A"+QString::number(i+1);//设置要操作的单元格，如A1
                QString Y="B"+QString::number(i+1);
                cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);//获取单元格
                cellY = worksheet->querySubObject("Range(QVariant, QVariant)",Y);
                cellX->dynamicCall("SetValue(const QVariant&)",QVariant(1));//设置单元格的值
                cellY->dynamicCall("SetValue(const QVariant&)",QVariant(2));
            }

            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filepath));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel=NULL;
        }

        QMessageBox::about(NULL, tr("提示"), tr("excel已经导出"));
}

void Widget::readExl()
{

    //   QAxSelect select;
    //   select.show();
       QAxWidget excel("Excel.Application");
       excel.setProperty("Visible", true);//显示当前窗口
       //excel.setProperty("Caption","Invoke Microsoft Excel");//更改标题栏
       QAxObject * workbooks = excel.querySubObject("WorkBooks");//获取excel的集合指针
       QAxObject *workbook = workbooks->querySubObject("Open (const QString &)",QString("c:/89.xls"));//打开指定路径的xls文档
       QAxObject * worksheets = workbook->querySubObject("WorkSheets");//获取sheets的集合指针
       int intCount = worksheets->property("Count").toInt();//获取sheets的数量
       for (int i = 1; i <= intCount; i++)
       {
           int intVal;
           QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", i);//获取指定位置的sheet页面
           qDebug() << i << worksheet->property("Name").toString();
           QAxObject * range = worksheet->querySubObject("Cells(1,1)");//获取sheet页面中的C(1,1)数据
           qDebug() <<i << range->property("Value").toString();
           intVal = range->property("Value").toInt();//保存该数据
           range->setProperty("Value", QVariant(intVal+1));//将数据加1并重新保存回C（1,1）
           QAxObject * range2 = worksheet->querySubObject("Range(C1)");//???????????
           intVal = range2->property("Value").toInt();
           range2->setProperty("Value", QVariant(intVal+1));
       }
       QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", 1);//打开第一个sheet
       QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
       QAxObject * rows = usedrange->querySubObject("Rows");
       QAxObject * columns = usedrange->querySubObject("Columns");
       int intRowStart = usedrange->property("Row").toInt();
       qDebug() << intRowStart;
       int intColStart = usedrange->property("Column").toInt();
       qDebug() << intColStart;
       int intCols = columns->property("Count").toInt();
       int intRows = rows->property("Count").toInt();

       for (int i = intRowStart; i < intRowStart + intRows; i++)
       {
           for (int j = intColStart; j < intColStart + intCols; j++)
           {
               QAxObject * range = worksheet->querySubObject("Cells(int,int)", i, j );
               qDebug() << i << j << range->property("Value").toString();
           }
       }
       excel.setProperty("DisplayAlerts", 0);
       workbook->dynamicCall("SaveAs (const QString&)", QString("c:/xlsbyqt.xls"));
       excel.setProperty("DisplayAlerts", 1);
       workbook->dynamicCall("Close (Boolean)", false);
       excel.dynamicCall("Quit (void)");

}

void Widget::readExlA()
{
   // 主要读取内容： 标题,工作表数目,工作表名称,起始行,起始列,行数,列数,单元格内容


    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", true);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", QString("E:/testa.xlsx"));
    QVariant title_value = excel.property("Caption");  //获取标题
    qDebug()<<QString("excel title : ")<<title_value;
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用WorkSheets

    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目
    qDebug()<<QString("sheet count : ")<<sheet_count;
        for(int i=1; i<=sheet_count; i++)
        {
            QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", i);  //Sheets(int)也可换用Worksheets(int)
            QString work_sheet_name = work_sheet->property("Name").toString();  //获取工作表名称
            QString message = QString("sheet ")+QString::number(i, 10)+ QString(" name");
            qDebug()<<message<<work_sheet_name;
        }
    if(sheet_count > 0)
    {
    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
    QAxObject *used_range = work_sheet->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");
    int row_start = used_range->property("Row").toInt();  //获取起始行
    int column_start = used_range->property("Column").toInt();  //获取起始列
    int row_count = rows->property("Count").toInt();  //获取行数
    int column_count = columns->property("Count").toInt();  //获取列数
    for(int i=row_start; i < row_count;i++)
    {
        for(int j=column_start;j<column_count;j++)
        {
            QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
            QVariant cell_value = cell->property("Value");  //获取单元格内容
            QString message = QString("row-")+QString::number(i, 10)+QString("-column-")+QString::number(j, 10)+QString(":");
            qDebug()<<message<<cell_value;
        }
    }
   }

}

void Widget::writeExlA()
{

}

Widget::~Widget()
{
    delete ui;
}


/**
 *QAxWidget excel("Excel.Application");
1) 显示当前窗口：
excel.setProperty("Visible", true);
2) 更改 Excel 标题栏：
excel.setProperty("Caption", "Invoke Microsoft Excel");
3) 添加新工作簿：
QAxObject * workbooks = excel.querySubObject("WorkBooks");
workbooks->dynamicCall("Add");
4) 打开已存在的工作簿：
workbooks->dynamicCall("Open (const QString&)", QString("c:/test.xls"));
5) 获取活动工作簿：
QAxObject * workbook = excel.querySubObject("ActiveWorkBook");
6) 获取所有的工作表：
QAxObject * worksheets = workbook->querySubObject("WorkSheets");
7) 获取工作表数量：
int intCount = worksheets->property("Count").toInt();
8) 获取第一个工作表：
QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", 1);
9) 获取cell的值：
QAxObject * range = worksheet->querySubObject("Cells(int,int)", 1, 1 );

以上操作不一定每个都可用(都好用)，仅作为参考吧。起码我第9)个就没成功，不知为何？后来我改用了Range(QVariant, QVariant)的方法来替代它。

最后附上我用QAxBase::generateDocumentation()方法获得的对应于Excel.Application的带Qt数据类型的QAxObject或者QAxWidget中可用的属性、信号和槽列表(虽然我还不大会用，不过参考下还是很好的):
 */

void Widget::on_pushButton_clicked()
{
       writeExl();
}

void Widget::on_pushButton_2_clicked()
{
    readExl();
    //readExlA();
}

void Widget::exlMode()
{
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", true);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Open(const QString&)", "c:\\testa.xls");
    excel.setProperty("Caption", "Qt Excel");
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用WorkSheets

    //删除工作表（删除第一个）
    QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
    first_sheet->dynamicCall("delete");

    //插入工作表（插入至最后一行）
    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目
    QAxObject *last_sheet = work_sheets->querySubObject("Item(int)", sheet_count);
    QAxObject *work_sheet = work_sheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
    last_sheet->dynamicCall("Move(QVariant)", work_sheet->asVariant());

    work_sheet->setProperty("Name", "Qt Sheet");  //设置工作表名称

    //操作单元格（第2行第2列）
    QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", 2, 2);
    cell->setProperty("Value", "Java C++ C# PHP Perl Python Delphi Ruby");  //设置单元格值
    cell->setProperty("RowHeight", 50);  //设置单元格行高
    cell->setProperty("ColumnWidth", 30);  //设置单元格列宽
    cell->setProperty("HorizontalAlignment", -4108); //左对齐（xlLeft）：-4131  居中（xlCenter）：-4108  右对齐（xlRight）：-4152
    cell->setProperty("VerticalAlignment", -4108);  //上对齐（xlTop）-4160 居中（xlCenter）：-4108  下对齐（xlBottom）：-4107
    cell->setProperty("WrapText", true);  //内容过多，自动换行
    //cell->dynamicCall("ClearContents()");  //清空单元格内容

    QAxObject* interior = cell->querySubObject("Interior");
    interior->setProperty("Color", QColor(0, 255, 0));   //设置单元格背景色（绿色）

    QAxObject* border = cell->querySubObject("Borders");
    border->setProperty("Color", QColor(0, 0, 255));   //设置单元格边框色（蓝色）

    QAxObject *font = cell->querySubObject("Font");  //获取单元格字体
    font->setProperty("Name", QStringLiteral("华文彩云"));  //设置单元格字体
    font->setProperty("Bold", true);  //设置单元格字体加粗
    font->setProperty("Size", 20);  //设置单元格字体大小
    font->setProperty("Italic", true);  //设置单元格字体斜体
    font->setProperty("Underline", 2);  //设置单元格下划线
    font->setProperty("Color", QColor(255, 0, 0));  //设置单元格字体颜色（红色）

    //设置单元格内容，并合并单元格（第5行第3列-第8行第5列）
    QAxObject *cell_5_6 = work_sheet->querySubObject("Cells(int,int)", 5, 3);
    cell_5_6->setProperty("Value", "Java");  //设置单元格值
    QAxObject *cell_8_5 = work_sheet->querySubObject("Cells(int,int)", 8, 5);
    cell_8_5->setProperty("Value", "C++");

    QString merge_cell;
    merge_cell.append(QChar(3 - 1 + 'A'));  //初始列
    merge_cell.append(QString::number(5));  //初始行
    merge_cell.append(":");
    merge_cell.append(QChar(5 - 1 + 'A'));  //终止列
    merge_cell.append(QString::number(8));  //终止行
    QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", merge_cell);
    merge_range->setProperty("HorizontalAlignment", -4108);
    merge_range->setProperty("VerticalAlignment", -4108);
    merge_range->setProperty("WrapText", true);
    merge_range->setProperty("MergeCells", true);  //合并单元格
    //merge_range->setProperty("MergeCells", false);  //拆分单元格

    //work_book->dynamicCall("Save()");  //保存文件（为了对比test与下面的test2文件，这里不做保存操作）
    work_book->dynamicCall("SaveAs(const QString&)", "c:\\test2.xls");  //另存为另一个文件
    work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
    excel.dynamicCall("Quit(void)");  //退出
}
 void Widget::formatExcel()
 {
     QAxObject excel("Excel.Application");
     excel.setProperty("Visible", true);
     QAxObject *work_books = excel.querySubObject("WorkBooks");
     work_books->dynamicCall("Open(const QString&)", "c:\\testa.xls");
     excel.setProperty("Caption", "Qt Excel");
     QAxObject *work_book = excel.querySubObject("ActiveWorkBook");

     //QAxObject * work_sheet = work_sheets->querySubObject("Worksheets(int)", 1);//打开第一个sheet
     QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);  //Sheets(int)也可换用Worksheets(int)


     //操作单元格（第2行第2列）
     QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", 2, 2);
     cell->setProperty("Value", "Java C++ C# PHP Perl Python Delphi Ruby");  //设置单元格值
     cell->setProperty("RowHeight", 50);  //设置单元格行高
     cell->setProperty("ColumnWidth", 30);  //设置单元格列宽
     cell->setProperty("HorizontalAlignment", -4108); //左对齐（xlLeft）：-4131  居中（xlCenter）：-4108  右对齐（xlRight）：-4152
     cell->setProperty("VerticalAlignment", -4108);  //上对齐（xlTop）-4160 居中（xlCenter）：-4108  下对齐（xlBottom）：-4107
     cell->setProperty("WrapText", true);  //内容过多，自动换行
     //cell->dynamicCall("ClearContents()");  //清空单元格内容


     cellColor(cell,true,20,true,2,"red");
     cellBackAndSideColor(cell,"green","black");


     QAxObject *cell_5_6 = work_sheet->querySubObject("Cells(int,int)", 5, 3);
     cell_5_6->setProperty("Value", "Java");  //设置单元格值
     mergeCell(work_sheet,5,3,5,6);

     //merge_range->setProperty("MergeCells", false);  //拆分单元格
    // work_book->dynamicCall("SaveAs(const QString&)", "c:\\test3.xls");  //另存为另一个文件
    // work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
    // excel.dynamicCall("Quit(void)");  //退出

 }
 //合并单元格
void Widget::mergeCell(QAxObject *work_sheet,int s_row,int s_col,int e_row,int e_col)
{
    QString merge_cell;
    merge_cell.append(QChar(s_col - 1 + 'A'));  //初始列
    merge_cell.append(QString::number(s_row));  //初始行
    merge_cell.append(":");
    merge_cell.append(QChar(e_col - 1 + 'A'));  //终止列
    merge_cell.append(QString::number(e_row));  //终止行
    QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", merge_cell);
    merge_range->setProperty("HorizontalAlignment", -4108);
    merge_range->setProperty("VerticalAlignment", -4108);
    merge_range->setProperty("WrapText", true);
    merge_range->setProperty("MergeCells", true);
}
 //设置字体颜色
void Widget::cellColor(QAxObject *cell,bool bold,int size,bool italic,int line,QString color)
{
    QAxObject *font = cell->querySubObject("Font");  //获取单元格字体
    font->setProperty("Name", QStringLiteral("华文彩云"));  //设置单元格字体
    font->setProperty("Bold", bold);  //设置单元格字体加粗
    font->setProperty("Size", size);  //设置单元格字体大小
    font->setProperty("Italic", italic);  //设置单元格字体斜体
    font->setProperty("Underline", line);  //设置单元格下划线
    if (color.compare("red") == 0){
        font->setProperty("Color", QColor(255, 0, 0));  //设置单元格字体颜色（红色）
    }else{
        font->setProperty("Color", QColor(0, 0, 0));  //设置单元格字体颜色（黑色）
    }

}
//设置单元格背景色与边框色
void Widget::cellBackAndSideColor(QAxObject *cell,QString backColor,QString sideColor)
{
    QAxObject* interior = cell->querySubObject("Interior");
    if(backColor.compare("red") == 0){
         interior->setProperty("Color", QColor(255, 0, 0));
    }else if(backColor.compare("green") == 0){
         interior->setProperty("Color", QColor(0, 255, 255));   //设置单元格背景色（绿色）
    }


    QAxObject* border = cell->querySubObject("Borders");
    if(sideColor.compare("black") == 0)
    {
       border->setProperty("Color", QColor(0, 0,0));   //设置单元格边框色（黑色）
    }else{
        border->setProperty("Color", QColor(0, 0,255));   //设置单元格边框色（蓝色）
    }

}

void Widget::on_pushButton_3_clicked()
{
   // exlMode();
    formatExcel();
}
