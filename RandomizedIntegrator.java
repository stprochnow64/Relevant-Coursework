public class RandomizedIntegrator{

  private double upperBound;
  private double lowerBound;
  private static double[] polynomial;

	public static double throwDarts(double lowerBound, double upperBound, double[] polynomial){
    double numOfHits = 0;
    double numOfDarts = 10000000;
    double finalAnswer = 0;
    double widthOfRect = (upperBound - lowerBound);
    double lowBoundOfHeight = findMin(lowerBound, upperBound, polynomial);
    double highBoundOfHeight = findMax(lowerBound, upperBound, polynomial);
    double heightOfRect = (highBoundOfHeight - lowBoundOfHeight);

    for (int i = 0; i <= numOfDarts; i++) {
      double x = (Math.random() * widthOfRect) + lowerBound;
      double y = (Math.random() * heightOfRect) + lowBoundOfHeight;

      double heightOfPolyAtPoint = polyFunction(x, polynomial);

      if (y <= heightOfPolyAtPoint && y > 0 ){
        numOfHits++;
      }
      else if (y >= heightOfPolyAtPoint && y < 0){
        numOfHits--;
      }

    }

    finalAnswer += ((numOfHits / numOfDarts) * heightOfRect * widthOfRect);
    return finalAnswer;
	}

  public static double findMax(double lowerBound, double upperBound, double[] polynomial){
    double max = 0;
    double test = 0;
    for (int i = 0; i <= 1000000; i++){
      double x = (Math.random() * (upperBound - lowerBound)) + lowerBound;

      double current = polynomial[0];
      for (int j = 1; j < polynomial.length; j++){
        current += polynomial[j] * Math.pow(x, j);
      }


      if (i == 0){
        max = current;
      }
      else{
        if (current >= max){
          max = current;
        }
      }
      if (max < 0){
        max = 0;
      }

    }
    return max;
  }

  public static double findMin(double lowerBound, double upperBound, double[] polynomial){
    double min = 0;
    double test = 0;
    for (int i = 0; i <= 1000000; i++){
      double x = (Math.random() * (upperBound - lowerBound)) + lowerBound;


      double current = polynomial[0];
      for (int j = 1; j < polynomial.length; j++){
        current += polynomial[j] * Math.pow(x, j);
      }


      if (i == 0){
        min = current;
      }
      else{
        if (current <= min){
          min = current;
        }
      }
      if (min > 0){
        min = 0;
      }
    }
    return min;
  }

	public static double polyFunction(double x, double[] polynomial){
		double sum = 0;
		for(int i = 0; i < polynomial.length; i++){
			sum += polynomial[i] * Math.pow(x, i);
		}
		return sum;
	}

  public static void main(String[] args){
    double lowerBound = Double.parseDouble(args[args.length - 2]);
    double upperBound = Double.parseDouble(args[args.length - 1]);
    double[] polynomial = new double[args.length - 2];
    int lengthOfEnteredData = args.length;

     if (lengthOfEnteredData < 3){
       System.out.println("INSUFFICIENT DATA");
     }
     else if (lowerBound > upperBound){
       System.out.println("INAPPROPRIATE BOUNDS");
     }
     else {
        for(int i = 0; i <= args.length  - 3; i++){
            polynomial[i] = Double.parseDouble(args[i]);
        }
        double output = throwDarts(lowerBound, upperBound, polynomial);
        System.out.println("Final answer: " + output);
    }
  }

}
