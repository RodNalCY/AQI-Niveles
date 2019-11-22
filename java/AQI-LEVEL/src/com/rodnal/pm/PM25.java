package com.rodnal.pm;

public class PM25 {

  public static void main(String[] args) {
    
    calcularAQIPM25(5);

  }

  public static double calcularAQIPM25(double concentration) {

    double conc = concentration;
    double c;
    double AQI;
    c = Math.floor(10 * conc) / 10;

    if (c >= 0 && c < 12.1) {
      AQI = Linear(50, 0, 12, 0, c);
    } else if (c >= 12.1 && c < 35.5) {
      AQI = Linear(100, 51, 35.4, 12.1, c);
    } else if (c >= 35.5 && c < 55.5) {
      AQI = Linear(150, 101, 55.4, 35.5, c);
    } else if (c >= 55.5 && c < 150.5) {
      AQI = Linear(200, 151, 150.4, 55.5, c);
    } else if (c >= 150.5 && c < 250.5) {
      AQI = Linear(300, 201, 250.4, 150.5, c);
    } else if (c >= 250.5 && c < 350.5) {
      AQI = Linear(400, 301, 350.4, 250.5, c);
    } else if (c >= 350.5 && c < 500.5) {
      AQI = Linear(500, 401, 500.4, 350.5, c);
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
