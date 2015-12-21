unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    Image1: TImage;
    OpenDialog1: TOpenDialog;
    Button1: TButton;
    Memo1: TMemo;
    Button2: TButton;
    Button3: TButton;
    SaveDialog1: TSaveDialog;
    Bevel1: TBevel;
    Edit1: TEdit;
    Edit2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Bevel2: TBevel;
    Label3: TLabel;
    Edit3: TEdit;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.Button1Click(Sender: TObject);
begin
	if form1.OpenDialog1.Execute then
        	form1.Image1.Picture.LoadFromFile(form1.opendialog1.filename);
end;                                                                    

procedure TForm1.Button2Click(Sender: TObject);
var
	x : integer;
        y : integer;
begin
	form1.Memo1.Clear;
        if (form1.CheckBox1.Checked = false) and (form1.CheckBox2.Checked = false) then
        	form1.Memo1.Lines.Add('void ' + edit3.text + '(void)');
        if (form1.CheckBox1.Checked = true) and (form1.CheckBox2.Checked = false) then
        	form1.Memo1.Lines.Add('void ' + edit3.text + '(unsigned char ' + edit1.text + ')');
        if (form1.CheckBox1.Checked = false) and (form1.CheckBox2.Checked = true) then
        	form1.Memo1.Lines.Add('void ' + edit3.text + '(unsigned char ' + edit2.text + ')');
        if (form1.CheckBox1.Checked = true) and (form1.CheckBox2.Checked = true) then
        	form1.Memo1.Lines.Add('void ' + edit3.text + '(unsigned char ' + edit1.text + ', unsigned char ' + edit2.text + ')');
        form1.Memo1.Lines.Add('{');
        for x := 0 to 159 do
        begin
        	for y := 0 to 79 do
                begin
                	if form1.Image1.Canvas.Pixels[x, y] = clblack then
                        	form1.Memo1.Lines.Add('    lcd_graphics_plot_pixel(' + edit1.text + ' + ' + inttostr(x+1) + ', ' + edit2.text + ' + ' + inttostr(y+1) + ', PIXEL_ON);');
                end;
        end;
        form1.Memo1.Lines.Add('}');
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
	if form1.SaveDialog1.Execute then
        	form1.Memo1.Lines.SaveToFile(form1.SaveDialog1.FileName);
end;

end.
