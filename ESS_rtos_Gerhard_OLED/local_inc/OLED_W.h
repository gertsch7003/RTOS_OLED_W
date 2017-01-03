#ifndef OLED_W_H_
#define OLED_W_H_

#define SSD1306_96_39
#define SSD1306_LCDWIDTH                  96
#define SSD1306_LCDHEIGHT                 39

#define CHAR_WIDTH                         6
#define CHAR_HEIGHT                        8

// Commandos zur Displaysteuerung
#define SSD1306_DISPLAYOFF				0xAE
#define SSD1306_SETDISPLAYCLOCKDIV		0xD5
#define SSD1306_SETMULTIPLEX				0xA8
#define SSD1306_SETDISPLAYOFFSET			0xD3
#define SSD1306_SETSTARTLINE				0x40
#define SSD1306_CHARGEPUMP				0x8D

#define SSD1306_SETSEGMENTREMAP			0xA1  // ja
#define SSD1306_SEGREMAP					0xA0

#define SSD1306_COMSCANDEC				0xC8
#define SSD1306_SETCOMPINS				0xDA
#define SSD1306_SETCONTRAST 				0x81
#define SSD1306_SETPRECHARGE 			0xD9
#define SSD1306_SETVCOMDETECT 			0xDB
#define SSD1306_DISPLAYALLON_RESUME		0xA4
#define SSD1306_NORMALDISPLAY 			0xA6
#define SSD1306_DISPLAYON 				0xAF

#define SSD1306_DISPLAYALLON 			0xA5
#define SSD1306_INVERTDISPLAY 			0xA7
#define SSD1306_SETLOWCOLUMN 			0x00
#define SSD1306_SETHIGHCOLUMN 			0x10
#define SSD1306_MEMORYMODE 				0x20
#define SSD1306_COLUMNADDR 				0x21
#define SSD1306_PAGEADDR   				0x22
#define SSD1306_COMSCANINC 				0xC0
#define SSD1306_SEGREMAP 				0xA0
#define SSD1306_EXTERNALVCC				0x1
#define SSD1306_SWITCHCAPVCC				0x2

#define SSD1306_ACTIVATE_SCROLL			0x2F
#define SSD1306_DEACTIVATE_SCROLL 		0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL	0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 	0x27
#define SSD1306_VERT_RIGHT_HORI_SCROLL 	0x29
#define SSD1306_VERT_LEFT_HORI_SCROLL 	0x2A

//char* LAUFTEXT =
char* LAUFTEXT =
		"STAR WARS       "
				"EPISODE I       "
				"DIE DUNKLE BEDROHUNG            "
				"Die Galaktische Republik wird von Unruhen erschuettert. Die Besteuerung der Handelsrouten zu weit entfernten Sternensystemen ist der Ausloeser."
				"In der Hoffnung, die Angelegenheit durch eine Blockade mit maechtigen Kampfschiffen zu beseitigen, hat die unersaettliche Handelsfoederation jeglichen Transport zu dem kleinen Planeten Naboo eingestellt."
				"Waehrend der Kongress der Republik endlose Debatten ueber diese beunruhigende Kette von Ereignissen fuehrt, entsandte der Oberste Kanzler insgeheim zwei Jedi-Ritter, Waechter des Friedens und der Gerechtigkeit, um den Konflikt beizulegen....";

// Bild das im Beispielprojekt f�r das Display enthalten war
//unsigned char pic[] =
unsigned char pic[] = { 0xFF, 0xFF, 0x3F, 0x1F, 0x0F, 0x07, 0x07, 0x03, 0x03,
		0x03, 0x03, 0x03, 0x03, 0x03, 0xE3, 0xE3, 0x03, 0x03, 0x03, 0x03, 0xC3,
		0xE3, 0xC3, 0x03, 0x03, 0x03, 0x03, 0xE3, 0xE3, 0x03, 0x03, 0x03, 0x03,
		0x03, 0x03, 0x03, 0x03, 0xC3, 0xC3, 0x63, 0x63, 0x63, 0x63, 0x63, 0xE3,
		0xE3, 0x03, 0x03, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0xE3, 0xE3,
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xE3, 0xE3, 0x03, 0x03,
		0x03, 0xC3, 0xC3, 0x63, 0x63, 0x63, 0x63, 0xC3, 0xC3, 0x03, 0x03, 0x03,
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07, 0x0F, 0x1F,
		0x3F, 0xFF, 0xFF,

		0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x03, 0x1F, 0xF8, 0xC0, 0xFE, 0x1F, 0x00, 0x1F, 0xFE,
		0xC0, 0xF8, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x3F, 0xFF, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0x00, 0x00,
		0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0xFF, 0xFF, 0x00, 0x00, 0x80,
		0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x3F, 0xFF, 0xC0,
		0x80, 0x80, 0x80, 0x80, 0xC0, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

		0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
		0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x01, 0x00, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x81, 0x81, 0x01, 0x01, 0x80, 0x80,
		0x01, 0x01, 0xC1, 0xC1, 0xC1, 0x81, 0x01, 0x01, 0x01, 0x00, 0x00, 0x81,
		0xC1, 0xC1, 0xC1, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

		0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x31,
		0x31, 0x31, 0x31, 0x31, 0x31, 0xFF, 0xFF, 0x00, 0x00, 0xF0, 0xF8, 0x1C,
		0x0C, 0x0C, 0x1C, 0xF8, 0xF0, 0x00, 0x0C, 0x0C, 0x18, 0xFC, 0xFC, 0x00,
		0x00, 0x04, 0x8C, 0xF8, 0x70, 0xE0, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xFD,
		0x00, 0x00, 0xFF, 0xFF, 0x03, 0x1F, 0xFC, 0xC0, 0x00, 0xC0, 0xFC, 0x1F,
		0x03, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x3C, 0xF0, 0xC0,
		0x00, 0xC0, 0xF0, 0x3C, 0x0C, 0x00, 0xF0, 0xF8, 0x1C, 0x0C, 0x0C, 0x18,
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

		0xFF, 0xFE, 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x86, 0x86,
		0x86, 0x86, 0x86, 0x86, 0x86, 0x87, 0x87, 0x80, 0x80, 0x81, 0x83, 0x87,
		0x86, 0x86, 0x87, 0x83, 0x81, 0x80, 0x80, 0x80, 0x80, 0x87, 0x87, 0x80,
		0x80, 0x86, 0x87, 0x81, 0x80, 0x80, 0x87, 0x87, 0x80, 0x80, 0x87, 0x87,
		0x80, 0x80, 0x87, 0x87, 0x80, 0x80, 0x81, 0x87, 0x86, 0x87, 0x81, 0x80,
		0x80, 0x87, 0x87, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x87,
		0x9F, 0xBF, 0xB1, 0xB0, 0x80, 0x80, 0x81, 0x83, 0x87, 0x86, 0x86, 0x83,
		0x87, 0x87, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFE, 0xFF };

// Pixelschrift mit Zeichenhoehe 8 und Zeichenbreite: 6
// Haendisch umgewandelt von der Standardpixelschriftvorlage von: http://www.pentacom.jp/soft/ex/font/edit_canvas.html
//unsigned char chars[] =
unsigned char chars[] = { 0x00, 0x00, 0x00, 0x00,
		0x00,  //<Blank>
		0x00, 0x00, 0xBE, 0x00, 0x00,
		0x00,  //!
		0x00, 0x06, 0x00, 0x06, 0x00,
		0x00,  //"
		0x00, 0x48, 0xFC, 0x48, 0xFC,
		0x48,  //#
		0x00, 0x4C, 0x92, 0xFF, 0x92,
		0x64,  //$
		0x00, 0x7C, 0x82, 0x00, 0x00,
		0x00,  //(
		0x00, 0x82, 0x7C, 0x00, 0x00,
		0x00,  //)
		0x00, 0x2A, 0x1C, 0x08, 0x1C,
		0x2A,  //*
		0x00, 0x10, 0x10, 0x7C, 0x10,
		0x10,  //+
		0x00, 0x00, 0x80, 0x40, 0x00,
		0x00,  //,
		0x00, 0x10, 0x10, 0x10, 0x10,
		0x00,  //-
		0x00, 0x00, 0x80, 0x00, 0x00,
		0x00,  //.
		0x00, 0x80, 0x60, 0x18, 0x06,
		0x00,  ///
		0x00, 0x7C, 0x82, 0x82, 0x82,
		0x7C,  //0
		0x00, 0x00, 0x84, 0xFE, 0x80,
		0x00,  //1
		0x00, 0xC4, 0xA2, 0x92, 0x92,
		0x8C,  //2
		0x00, 0x44, 0x82, 0x92, 0x92,
		0x6C,  //3
		0x00, 0x30, 0x28, 0x24, 0xFE,
		0x20,  //4
		0x00, 0x9E, 0x92, 0x92, 0x62,
		0x00,  //5
		0x00, 0x7C, 0x92, 0x92, 0x92,
		0x64,  //6
		0x00, 0x02, 0xC2, 0x32, 0x0A,
		0x06,  //7
		0x00, 0x6C, 0x92, 0x92, 0x92,
		0x6C,  //8
		0x00, 0x0C, 0x92, 0x92, 0x52,
		0x3C,  //9
		0x00, 0x00, 0x44, 0x00, 0x00,
		0x00,  //:
		0x00, 0x00, 0x80, 0x44, 0x00,
		0x00,  //;
		0x00, 0x10, 0x28, 0x44, 0x82,
		0x00,  //<
		0x00, 0x28, 0x28, 0x28, 0x28,
		0x28,  //=
		0x00, 0x82, 0x44, 0x28, 0x10,
		0x00,  //>
		0x00, 0x04, 0x02, 0xB2, 0x0A,
		0x04,  //?
		0x00, 0x78, 0x84, 0x64, 0x54,
		0x78,  //@
		0x00, 0xFC, 0x12, 0x12, 0x12,
		0xFC,  //A
		0x00, 0xFE, 0x92, 0x92, 0x92,
		0x6C,  //B
		0x00, 0x7C, 0x82, 0x82, 0x82,
		0x44,  //C
		0x00, 0xFE, 0x82, 0x82, 0x82,
		0x7C,  //D
		0x00, 0xFE, 0x92, 0x92, 0x92,
		0x82,  //E
		0x00, 0xFE, 0x12, 0x12, 0x12,
		0x02,  //F
		0x00, 0x7C, 0x82, 0x82, 0x92,
		0x74,  //G
		0x00, 0xFE, 0x10, 0x10, 0x10,
		0xFE,  //H
		0x00, 0x82, 0x82, 0xFE, 0x82,
		0x82,  //I
		0x00, 0x60, 0x80, 0x80, 0x80,
		0x7E,  //J
		0x00, 0xFE, 0x10, 0x28, 0x44,
		0x82,  //K
		0x00, 0xFE, 0x80, 0x80, 0x80,
		0x80,  //L
		0x00, 0xFE, 0x04, 0x08, 0x04,
		0xFE,  //M
		0x00, 0xFE, 0x08, 0x10, 0x20,
		0xFE,  //N
		0x00, 0x7C, 0x82, 0x82, 0x82,
		0x7C,  //O
		0x00, 0xFE, 0x12, 0x12, 0x12,
		0x0C,  //P
		0x00, 0x3C, 0x42, 0x42, 0xC2,
		0xBC,  //Q
		0x00, 0xFE, 0x12, 0x32, 0x52,
		0x8C,  //R
		0x00, 0x4C, 0x92, 0x92, 0x92,
		0x64,  //S
		0x00, 0x02, 0x02, 0xFE, 0x02,
		0x02,  //T
		0x00, 0x7E, 0x80, 0x80, 0x80,
		0x7E,  //U
		0x00, 0x3E, 0x40, 0x80, 0x40,
		0x3E,  //V
		0x00, 0x3E, 0xC0, 0x38, 0xC0,
		0x3E,  //W
		0x00, 0xC6, 0x28, 0x10, 0x28,
		0xC6,  //X
		0x00, 0x06, 0x08, 0xF0, 0x08,
		0x06,  //Y
		0x00, 0xC2, 0xA2, 0x92, 0x8A,
		0x86,  //Z
		0x00, 0x70, 0x88, 0x88, 0xF8,
		0x80,  //a
		0x00, 0xFE, 0x88, 0x88, 0x70,
		0x00,  //b
		0x00, 0x70, 0x88, 0x88, 0x88,
		0x00,  //c
		0x00, 0x70, 0x88, 0x88, 0xFE,
		0x00,  //d
		0x00, 0x70, 0xA8, 0xA8, 0xA8,
		0xB0,  //e
		0x00, 0x08, 0xFE, 0x09, 0x09,
		0x00,  //f
		0x00, 0x58, 0xA4, 0xA4, 0x78,
		0x00,  //g
		0x00, 0xFE, 0x08, 0x08, 0xF0,
		0x00,  //h
		0x00, 0x00, 0x88, 0xFA, 0x80,
		0x00,  //i
		0x00, 0x40, 0x80, 0x80, 0x7A,
		0x00,  //j
		0x00, 0xFE, 0x20, 0x30, 0xC8,
		0x00,  //k
		0x00, 0x00, 0xFE, 0x00, 0x00,
		0x00,  //l
		0x00, 0xF8, 0x08, 0xF0, 0x08,
		0xF0,  //m
		0x00, 0xF8, 0x08, 0x08, 0xF0,
		0x00,  //n
		0x00, 0x70, 0x88, 0x88, 0x88,
		0x70,  //o
		0x00, 0xFC, 0x44, 0x44, 0x38,
		0x00,  //p
		0x00, 0x38, 0x44, 0x44, 0xF8,
		0x00,  //q
		0x00, 0xF8, 0x10, 0x08, 0x08,
		0x00,  //r
		0x00, 0x90, 0xA8, 0xA8, 0x48,
		0x00,  //s
		0x00, 0x08, 0x7E, 0x88, 0x88,
		0x00,  //t
		0x00, 0x78, 0x80, 0x80, 0xF8,
		0x00,  //u
		0x00, 0x18, 0x60, 0x80, 0x60,
		0x18,  //v
		0x00, 0x38, 0xC0, 0x30, 0xC0,
		0x38,  //w
		0x00, 0x88, 0x50, 0x20, 0x50,
		0x88,  //x
		0x00, 0x3C, 0xA0, 0xA0, 0x7C,
		0x00,  //y
		0x00, 0x88, 0xC8, 0xA8, 0x98,
		0x08,  //z
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#endif /* OLED_W_H_ */

