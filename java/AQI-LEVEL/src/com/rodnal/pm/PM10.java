package com.rodnal.pm;

public class PM10 {

  public static void main(String[] args) {
    
    calcularAQIPM10(39);

  }

  public static double calcularAQIPM10(double concentration) {

    double conc = concentration;
    double c;
    double AQI;
    c = Math.floor(conc);

    if (c >= 0 && c < 55) {
		AQI = Linear(50, 0, 54, 0, c);
	} else if (c >= 55 && c < 155) {
		AQI = Linear(100, 51, 154, 55, c);
	} else if (c >= 155 && c < 255) {
		AQI = Linear(150, 101, 254, 155, c);
	} else if (c >= 255 && c < 355) {
		AQI = Linear(200, 151, 354, 255, c);
	} else if (c >= 355 && c < 425) {
		AQI = Linear(300, 201, 424, 355, c);
	} else if (c >= 425 && c < 505) {
		AQI = Linear(400, 301, 504, 425, c);
	} else if (c >= 505 && c < 605) {
		AQI = Linear(500, 401, 604, 505, c);
	} else {
      AQI = 1000;
    }
    System.out.println(AQI);
    return AQI;

  }

  private static double Linear(double AQIhigh, double AQIlow, double Conchigh, double Conclow, double concentration) {
    double linear;
    double conc = concentration;
    double a;
    a = (conc - Conclow) / (Conchigh - Conclow) * (AQIhigh - AQIlow) + AQIlow;
    linear = Math.round(a);
    return linear;
  }

}
