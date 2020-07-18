#define MEASURE_PERIOD 10000  // время периода измерения 10 с = 10000 мс
#define LF_ant_pin A0
#define MF_ant_pin A1
#define HF_ant_pin A2
#define AMPLIFICATION 1000 //Коэфф усил
#define NUM_MES 100 //Коэфф усил
#define DT 100 //период измерений
#define EMR_TRESHOLD 200 //200 мкВТ час/см^2 - опасная доза ЭМИ
#define BUTTOM_PIN 2

float low_freq_ant, mid_freq_ant, high_freq_ant;   // измеренные напряжения
float ant1, ant2, ant3;

void setup() {
  Serial.begin(9600);  // инициализируем порт, скорость 9600
  pinMode(LF_ant_pin, INPUT);
  pinMode(MF_ant_pin, INPUT);
  pinMode(HF_ant_pin, INPUT);
  pinMode(BUTTOM_PIN, INPUT);
}


void loop() {
  //получаем положение c ползунка-слайдера,
  //которое позволяет мерить в определенных диапазонах или во всех сразу
  int slider = read_slider_pos();
  //если кнопка начала измерений была нажата
  if (digitalRead(BUTTOM_PIN) == HIGH) { 
    uint32_t tmr = micros();
    float  EE_val = 0;
    //проводим измерения в течении 10 с
    while (micros() - tmr < MEASURE_PERIOD) {
      low_freq_ant += analogRead(LF_ant_pin)*5 /1023;
      mid_freq_ant += analogRead(MF_ant_pin)*5 /1023;
      high_freq_ant += analogRead(HF_ant_pin)*5 /1023;
      delay(DT);
    }
    //рассчитываем энергетическую экспозицию диапазона, заданного ползунком-слайдером
    switch (slider) {
      case 1: 
        //измерение общих частот
        EE_val = energ_expose(low_freq_ant, mid_freq_ant, high_freq_ant);
      case 2: 
        //измерение с антены низких частот
        EE_val = energ_expose(low_freq_ant, 0.0, 0.0);
      case 3: 
        //измерение с антены средних частот
        EE_val = energ_expose(0.0, mid_freq_ant, 0.0);
      case 4: 
        //измерение с антены высоких частот
        EE_val = energ_expose(0.0, 0.0, high_freq_ant);
    }
    //выводим результат на экран
    disp(EE_val);
    //устройство сообщает о тревоге (звуковой сигнал + надпись на экране),
    //если нормы СанПИН нарушены
    if (danger(EE_val))
      alarm(); 
  }
}

float energ_expose(float val1,float val2,float val3){
  //возвращает значение рассчитанной энергетической экспозиции
  float danger;
  //проведение усреднения
  val1 = val1 / NUM_MES; 
  val2 = val2 / NUM_MES;
  val3 = val3 / NUM_MES;
  //рассчет энергетической экспозиции
  danger = (pow(val1,2) + pow(val2,2) + pow(val3,2))*10 / AMPLIFICATION;
  return danger;
}

bool danger(float & val){
  //решает опасно ли такое ЭМИ
  if (val >= 200.0)
    return 1;
  else
    return 0;
}

void disp(float & val){
  //отображает на экране столбик измеренной экспозиции в диапазоне 0-250 мкВТ час/см^2
  } 

int read_slider_pos(){
  //считывает положение ползунка-слайдера
  //и конвертирует положение в значения от 1 до 4
  int coverted_place = 1;
  return coverted_place;
}

void alarm(){
  //включает пьезопищалку и мигание экрана с надписью ALARM
}
