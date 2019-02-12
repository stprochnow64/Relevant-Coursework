import java.util.Arrays;

public class BigInteger {

	private int[] number;
	private boolean positive;

  //Determining sign and num of digits as string

  public BigInteger(String val){
    int indexValue = 0;

    if (val.charAt(0) == '-'){
      positive = false;
      indexValue += 1;
    }
    else if (val.charAt(0) == '+'){
      positive = true;
      indexValue += 1;
    }
    else {
      this.positive = true;
    }

    int sumOfDigits = 0;
    for (int x = indexValue; x < val.length(); x++){
      char c = val.charAt(x);
      sumOfDigits += Character.getNumericValue(c);
    }
      if (sumOfDigits == 0){
        this.positive = true;
        val = "0";
      }

      else {
        while (val.charAt(indexValue) == '0'){
          indexValue++;
        }
      }

      String output = val.substring(indexValue, val.length());

      indexValue = 0;

      int stringLength = output.length();
      this.number = new int[stringLength];

      for (int i = this.number.length - 1; i >= 0; i--){
        char c = output.charAt(i);
        this.number[indexValue] = Character.getNumericValue(c);
        indexValue++;
      }
  }

  // returns a BigInteger whose value is this plus val
  public BigInteger add ( BigInteger val ){
    int shortestNumLength = 0;
    int longestNumLength = 0;
    if (this.positive == val.positive) {
      if (this.number.length >= val.number.length){
        shortestNumLength = val.number.length;
        longestNumLength = this.number.length;
      }
      else if (this.number.length < val.number.length) {
        shortestNumLength = this.number.length;
        longestNumLength = val.number.length;
      }
      int[] sumUntilShortest = new int [shortestNumLength + 1];
      for (int i = 0; i < shortestNumLength; i++){
        sumUntilShortest[i] = this.number[i] + val.number[i];
      }

      int[] finalResult = new int [longestNumLength + 1];
      for (int j = 0; j < shortestNumLength; j++){
        finalResult[j] = sumUntilShortest[j];
      }

      for (int k = shortestNumLength; k < longestNumLength; k++){
        if (this.number.length > val.number.length){
          finalResult[k] += this.number[k];
        }
        else if (this.number.length < val.number.length){
          finalResult[k] += val.number[k];
        }
        else {
          break;
        }
      }

      for (int x = 0; x < finalResult.length; x++){
        if (finalResult[x] >= 10){
          finalResult[x] -= 10;
          finalResult[x + 1] += 1;
        }
      }

      String output = "-";
			if(this.positive && val.positive){
				output = "";
			}
			for(int i = finalResult.length - 1; i >= 0; i--){
				output += finalResult[i];
			}

			return new BigInteger(output);

		}
    else{
			if(!val.positive && this.positive){
				val.positive = true;
				return this.subtract(val); //SUBTRACT

			}
      else{
				this.positive = true;
				return val.subtract(this); //SUBTRACT
			}
		}
	}

  // returns a BigInteger whose value is this minus val
  public BigInteger subtract ( BigInteger val ){

    if(this.positive && !val.positive){
      val.positive = true;
      return this.add(val); //ADD
    }
    if(!this.positive && val.positive){
      val.positive = false;
      return this.add(val); //ADD
    }
    if(val.number.length > this.number.length){
      return new BigInteger("-" + val.subtract(this));
    }

    int shortestNumLength = 0;
    int longestNumLength = 0;
      if (this.number.length >= val.number.length){
        shortestNumLength = val.number.length;
        longestNumLength = this.number.length;
      }
      else if (this.number.length < val.number.length) {
        shortestNumLength = this.number.length;
        longestNumLength = val.number.length;
      }

    //result is positive
    if (this.compareTo(val) >= 0) {

    int[] finalResult = new int[shortestNumLength + 1];
    		for(int i = 0; i < shortestNumLength; i++){
    			finalResult[i] = this.number[i] - val.number[i];
    		}
    		for(int i = shortestNumLength; i < longestNumLength; i++){
    			finalResult[i] = this.number[i];
    		}

    		for(int i = 0; i < finalResult.length; i++){
    			if(finalResult[i] < 0){
    				finalResult[i] += 10;
    				finalResult[i + 1] -= 1;
    			}
    		}
    String output = "";
      for(int i = finalResult.length - 1; i >= 0; i--){
      output += finalResult[i];
        }
        return new BigInteger(output);

      }

      //result is negative
      else {
        int[] finalResult = new int[shortestNumLength + 1];
            for(int i = 0; i < shortestNumLength; i++){
              finalResult[i] = val.number[i] - this.number[i];
            }
            for(int i = shortestNumLength; i < longestNumLength; i++){
              finalResult[i] = val.number[i];
            }

            for(int i = 0; i < finalResult.length; i++){
              if(finalResult[i] < 0){
                finalResult[i] += 10;
                finalResult[i + 1] -= 1;
              }
            }
      String output = "-";
      for(int i = finalResult.length - 1; i >= 0; i--){
          output += finalResult[i];
      }
      return new BigInteger(output);

      }
    }

  //returns a BigInteger multiplied by val
	public BigInteger multiply(BigInteger val){

    int shortestNumLength = 0;
    int longestNumLength = 0;

    //determines larger num
      if (this.number.length >= val.number.length){
        shortestNumLength = val.number.length;
        longestNumLength = this.number.length;
      }
      else if (this.number.length < val.number.length) {
        shortestNumLength = this.number.length;
        longestNumLength = val.number.length;
      }
		int[] finalResult = new int[shortestNumLength + longestNumLength];
		for(int i = 0; i < this.number.length; i++){
			for(int j = 0; j < val.number.length; j++){
				finalResult[i + j] = this.number[i] * val.number[j];
			}
		}
		for(int i = 0; i < finalResult.length; i++){
			if(finalResult[i] >= 10){
				while(finalResult[i] >= 10){
					finalResult[i] -= 10;
					finalResult[i + 1] += 1;
				}
			}
		}

    String output = "";

    if(this.positive != val.positive){
      output += "-";
    }

		for(int i = finalResult.length - 1; i >= 0; i--){
			output += finalResult[i];
		}

		return new BigInteger(output);
	}

  // returns a BigInteger whose value is this divided by val
  public BigInteger divide ( BigInteger val ){

    throw new UnsupportedOperationException();

  }

  // returns a BigInteger whose value is the remainder of this when divided by val
  public BigInteger remainder ( BigInteger val ){

    throw new UnsupportedOperationException();

  }

  // returns the decimal string represention of this BigInteger
  public String toString (){
  		String output = "";
      if (this.positive){
  		for(int i = this.number.length - 1; i >= 0; i--){
  			output += Integer.toString(this.number[i]);
  		}
    }
  		else{
  			output += "-";
        for(int i = this.number.length - 1; i >= 0; i--){
    			output += Integer.toString(this.number[i]);
  		}
    }
  		return output;
  	}

  // returns -1 or 0 or 1 according to whetner this BigInteger is numerically less than or equal to or greater than val
  public int compareTo ( BigInteger val ){
    int answer = 0;
    if (this.positive && (! val.positive)){
      answer = 1;
    }
    else if ((! this.positive) && val.positive){
      answer = -1;
    }
    else if(this.positive && val.positive){
      if (this.number.length > val.number.length){
        answer = 1;
      }
      else if(this.number.length < val.number.length){
        answer = -1;
      }
      else {
        for (int i = 0; i < this.number.length; i++){
          if (this.number[i] > val.number[i]){
            answer = 1;
          }
          else if (this.number[i] < val.number[i]){
            answer = -1;
          }
          else {
            answer = 0;
          }
        }
      }
      if ((! this.positive) && (! val.positive)){
        answer *= -1;
      }
    }
    return answer;


  }

// returns true iff x is a BigInteger whose value is numerically equal to this BigInteger
public boolean equals ( Object x ){


      String objAsString = x.toString();

      BigInteger objAsBigInt = new BigInteger(objAsString);

      if ((this.compareTo(objAsBigInt) == 0 ) && (objAsBigInt.positive = this.positive)){
        return true;
      }
      else {
        return false;
      }

  }
  public static final BigInteger ZERO = new BigInteger("0"); // a classwide constant for zero


  public static final BigInteger ONE = new BigInteger("1"); // a classwide constant for one


  public static final BigInteger TEN = new BigInteger("10"); // a classwide constant for ten

  // constructs, and returns, a BigInteger from a long
  public static BigInteger valueOf ( long val ){
    throw new UnsupportedOperationException();
  }

  public static void main (String[] args){

  }

}
