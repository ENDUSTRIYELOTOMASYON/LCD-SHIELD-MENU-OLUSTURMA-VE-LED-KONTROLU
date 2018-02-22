String menuItems[] = {"SARI", "YESIL", "KIRMIZI", "MAVI"};
// Gezinme butonu değişkenleri
int readKey;
int savedDistance = 0;
// Menü kontrol değişkenleri
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = 0;
// Menü ekranı için 3 özel karakter oluşturur
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};
#include <Wire.h>
#include <LiquidCrystal.h>
// LCD ekran pinlerini ayarlama
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  // Seri iletişim başlatır
  Serial.begin(9600);
  // LCD ekranını başlatıp temizliyoruz
  lcd.begin(16, 2);
  lcd.clear();
  //3 özel karakterin görevi oluşturur
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(3,OUTPUT);
}
void loop() {
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
}
// Bu fonksiyon, ekranda sığabilen 2 menü öğesi oluşturacaktır. Menü boyunca ilerledikçe değişecekler. Yukarı ve aşağı oklar geçerli menü konumunuzu gösterecektir.
void mainMenuDraw() {
  Serial.print(menuPage);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}
// Çağrıldığında, bu işlev geçerli imleci silecek ve cursorPosition ve menuPage değişkenlerine dayalı olarak yeniden çizecektir.
void drawCursor() {
  for (int x = 0; x < 2; x++) {     // Erases current cursor
    lcd.setCursor(0, x);
    lcd.print(" ");
  }
  // Menü ileri gidecek olacak şekilde ayarlanmıştır (menuPage 0 = Öğe 1 ve Öğe 2, menü Sayfası 1 = Öğe 2 ve Öğe 3, menü Sayfası 2 = Öğe 3 ve Öğe 4), dolayısıyla
  // imlecin nerede olması gerektiğini belirlemek için tek veya çift menü sayfasında olup olmadığınızı ve tek veya çift imleç konumunu görmeniz gerekir
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {  // Menü sayfası eşitse ve imleç konumu eşitse, imlecin 1. satırda olması gerektiği anlamına gelir
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  //Menü sayfası eşitse ve imleç konumu tek ise imlecin 2. satırda olması gerektiği anlamına gelir
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  // Menü sayfası tekse ve imleç konumu eşitse, imlecin 2. satırda olması gerektiği anlamına gelir
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // Menü sayfası tek ve imleç konumu tek ise imlecin 1. satırda olması gerektiği anlamına gelir
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}
void operateMainMenu() {
  int activeButton = 0;
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 0: // Buton 0 olarak döndüğünde, herhangi bir işlem yapılmaz
        break;
      case 1:  // Bu durum, "ileri" düğmesine basıldığında yürütülecektir
        button = 0;
        switch (cursorPosition) { // Burada seçilen durum, bulunduğunuz menü sayfasına ve imlecin bulunduğu yere bağlıdır.
          case 0:
            menuItem1();
            break;
          case 1:
            menuItem2();
            break;
          case 2:
            menuItem3();
            break;
          case 3:
            menuItem4();
            break;
        }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}
//Bu işlev, bir butona basma değerlendirildiğinde çağrılır. LCD ekran, A0'a bağlanan düğmelerdeki gerilim düşüşünü gözlemleyerek çalışır.
int evaluateButton(int x) {
  int result = 0;
  if (x < 50) {
    result = 1; // right
  } else if (x < 195) {
    result = 2; // up
  } else if (x < 380) {
    result = 3; // down
  } else if (x < 790) {
    result = 4; // left
  }
  return result;
}

//Menü öğelerinizden en az 1'inde ortak kullanım talimatları varsa, bu işlevi alt menüden arayabilirsiniz
// şleri biraz daha basitleştiren menüler. Birden fazla menü üzerinde ortak talimatlar veya işaretleriniz yoksa
// Bu boşluğu silebilirsiniz. Ayrıca, alt menü işlevlerinizden drawInstructions () işlev çağrılarını silmelisiniz.
void drawInstructions() {
  lcd.setCursor(0, 1); // Set cursor to the bottom line
  lcd.print("Use ");
  lcd.write(byte(1)); // Up arrow
  lcd.print("/");
  lcd.write(byte(2)); // Down arrow
  lcd.print(" buttons");
}
void menuItem1() { // İşlev, ana menüden 2. öğeyi seçtiğinizde yürütülür
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("SARI LED");
  digitalWrite(13,HIGH);
  
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Bu durum, "geri" düğmesine basıldığında yürütülecektir
        button = 0;
        activeButton = 1;
        digitalWrite(13,LOW);
        break;
    }
  }
}

void menuItem2() { // İşlev, ana menüden 2. öğeyi seçtiğinizde yürütülür
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("YESIL LED");
  digitalWrite(12,HIGH);
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Bu durum, "geri" düğmesine basıldığında yürütülecektir
        button = 0;
        activeButton = 1;
        digitalWrite(12,LOW);
        break;
    }
  }
}
void menuItem3() { // İşlev, ana menüden 3. öğeyi seçtiğinizde yürütülür
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("KIRMIZI LED");
  digitalWrite(11,HIGH);
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Bu durum, "geri" düğmesine basıldığında yürütülecektir
        button = 0;
        activeButton = 1;
        digitalWrite(11,LOW);
        break;
    }
  }
}
void menuItem4() { // İşlev, ana menüden 4. öğeyi seçtiğinizde yürütülür
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("MAVI LED");
  digitalWrite(3,HIGH);
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Bu durum, "geri" düğmesine basıldığında yürütülecektir
        button = 0;
        activeButton = 1;
        digitalWrite(3,LOW);
        break;
    }
  }
}

