//Объявление сенсоров
const uint8_t Laser_pin = A0;
const uint8_t Power_good = A1;
const uint8_t Service_input = A2;
const uint8_t U_battery = A3;
const uint8_t Dopler_pin = 2;
const uint8_t Limit_switch = 15;

//Переменные поворотного механизма
const uint8_t A = 3;
const uint8_t B = 4;
const uint8_t C = 5;
const uint8_t D = 6;

//Исполняемые модули
const uint8_t Anti_dog = 7;
const uint8_t Pepper_granade = 8;
const uint8_t Paint_granade = 9;
const uint8_t Light_and_noise = 10;
const uint8_t Net_gun = 14;
const uint8_t Electro_shocer = 16;

//Переменные программы
byte send_status = 0;
int val;
boolean shocer_status = false;
boolean charge_status = true;
boolean power_status = true;
boolean service_status = false;

int moving_delay = 100;

//Подпрограммы

//Остановка робота и систем
void RobotStop()
{
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
}

//Движение вверх
void RobotUp()
{
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
}

//Движение вниз
void RobotDown()
{
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
}

//Движение влево
void RobotLeft()
{
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
}

//Движение вправо
void RobotRight()
{
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
}

//Создание строба на выходе указанного пина (длительность 100 милисекунд)
void CreateStrob(uint8_t pin_number)
{
  digitalWrite(pin_number, HIGH);
  //delayMicroseconds(1000000);
  delay(400);
  digitalWrite(pin_number, LOW);
}

//Запись нулей на случай непреднамеренного срабатывания модулей
void StopGuns()
{
  digitalWrite(Anti_dog, LOW);
  digitalWrite(Pepper_granade, LOW);
  digitalWrite(Paint_granade, LOW);
  digitalWrite(Light_and_noise, LOW);
  digitalWrite(Net_gun, LOW);
  digitalWrite(Limit_switch, LOW);
  digitalWrite(Electro_shocer, LOW);
}

//Проверка состояния батареи
//Если всё хорошо - return true
boolean CheckBattery()
{
  if(analogRead(U_battery) < 750)
    return false;
  else
    return true;
}

/////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  pinMode(Laser_pin, INPUT);                      // конфигурируем цифровой вывод датчика как вход
  pinMode(Dopler_pin, INPUT);
  pinMode(Power_good, INPUT);
  pinMode(Service_input, INPUT);
  pinMode(Limit_switch, INPUT);

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(Anti_dog, OUTPUT);
  pinMode(Pepper_granade, OUTPUT);
  pinMode(Paint_granade, OUTPUT);
  pinMode(Light_and_noise, OUTPUT);
  pinMode(Net_gun, OUTPUT);
  pinMode(Electro_shocer, OUTPUT);
  
}

void loop()
{
  //Отправка сигнала, если сработал и доплер и лазер
  if(digitalRead(Dopler_pin) == HIGH && send_status == 0)
  {
     Serial.println("Moved detected: ");
     send_status = 1;
  }
  if(digitalRead(Dopler_pin) == LOW)
  {
     send_status = 0;
  }
   
  /*
  //Отправка сигнала, если сработал и доплер и лазер
  if(digitalRead(Laser_pin) == LOW && send_status == 0)
  {
    if(digitalRead(Dopler_pin) == HIGH)
    
       Serial.println("Moved detected: Dopler-ok,laser-ok ");
       send_status = 1;
  }
  
  if(digitalRead(Dopler_pin) == LOW)
  {
     send_status = 0;
  }
  */
  
  //----------------------------------------------------------
  //Проверка статуса батареи
  
  if(!CheckBattery() && charge_status)
  {
    Serial.println("Discharged batteries ");
    charge_status = false;
  }
  else if (CheckBattery() && !charge_status)
    charge_status = true;
  
  //----------------------------------------------------------
  //Проверка состояния Power good
  
  if(digitalRead(Power_good) == LOW && power_status)
  {
    Serial.println("Power supply is off");
    power_status = false;
  }
  else if (digitalRead(Power_good) == HIGH && !power_status)
    power_status = true;
  
  //----------------------------------------------------------
  //Проверка состояния сервисного режима
  
  if(digitalRead(Service_input) == HIGH && !service_status)
  {
    Serial.println("Robot in service");
    service_status = true;
  }
  else if (digitalRead(Service_input) == LOW && service_status)
    service_status = false;
  //----------------------------------------------------------  
  
  if (Serial.available())
  {
    val = Serial.read() - '0';
    
    //Движение платформы вверх
    if(val == 1)
    {
      RobotUp();
      delay(moving_delay);
      RobotStop();
    }
    
    //Движение платформы вних
    else if(val == 2)
    {
      RobotDown();
      delay(moving_delay);
      RobotStop();
    }
    
    //Движение платформы влево
    if(val == 3)
    {
      RobotLeft();
      delay(moving_delay);
      RobotStop();
    }
    
    //Движение платформы впрево
    if(val == 4)
    {
      RobotRight();
      delay(moving_delay);
      RobotStop();
    }
    
    //Включение Antidog
    if(val == 5)
    {
      CreateStrob(Anti_dog);
      //RobotStop();
    }
    
    //Пуск Перцовой гранаты
    if(val == 6)
    {
      CreateStrob(Pepper_granade);
      //RobotStop();
    }
    
    //Пуск Грнаты с краской
    if(val == 7)
    {
      CreateStrob(Paint_granade);
      //RobotStop();
    }
    
    //Включение-выключение Света и звука
    if(val == 8)
    {
      digitalWrite(Light_and_noise, !digitalRead(Light_and_noise));
      //RobotStop();
    }
    
    //Выбрасывание сетки
    if(val == 9)
    {
      digitalWrite(Net_gun, HIGH);
      //RobotStop();
    }
    
    //Стробирующий запуск Электрошокера
    if(val == 0)
    {
      if (shocer_status == false)
      {
        //delayMicroseconds(300000);
        delay(300);
        CreateStrob(Electro_shocer);  
      }
      else
      {
        CreateStrob(Electro_shocer);
      }
      
    }
    
  }
  else
  {
    StopGuns();
    Serial.flush();
  }
  
}
