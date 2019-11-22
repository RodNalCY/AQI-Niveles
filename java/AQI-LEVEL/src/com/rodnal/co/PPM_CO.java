package com.rodnal.co;

public class PPM_CO {

  public static void main(String[] args) {
    PPM_CO.calcularAQICO(4);
    //double x = Math.floor(10 * 7)/10;
    //System.out.println(x);
  }

  public static double calcularAQICO(double concentration) {

    double conc = concentration;
    double c;
    double AQI;
    c = Math.floor(10 * conc) / 10;

    if (c >= 0 && c < 4.5) {
      AQI = Linear(50, 0, 4.4, 0, c);
    } else if (c >= 4.5 && c < 9.5) {
      AQI = Linear(100, 51, 9.4, 4.5, c);
    } else if (c >= 9.5 && c < 12.5) {
      AQI = Linear(150, 101, 12.4, 9.5, c);
    } else if (c >= 12.5 && c < 15.5) {
      AQI = Linear(200, 151, 15.4, 12.5, c);
    } else if (c >= 15.5 && c < 30.5) {
      AQI = Linear(300, 201, 30.4, 15.5, c);
    } else if (c >= 30.5 && c < 40.5) {
      AQI = Linear(400, 301, 40.4, 30.5, c);
    } else if (c >= 40.5 && c < 50.5) {
      AQI = Linear(500, 401, 50.4, 40.5, c);
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
