/*
From the Wall Street Journal (February 24-25, 2018):
"You keep rolling a set of five regular dice until you get a set showing
exactly one six or a set showing zero sixes. When that happens,
you stop rolling the dice. You win if you stop on a roll with exactly one six.
You lose if you stop on a roll with no sixes. What is your probability of
winning?"

(1) Analyze three versions of the puzzle- with three dice, five dice,
and eight dice. (2) Confirm your analyses with a Java program, DiceSolver.
It should take one optional argument, namely, the number of dice to be rolled
(default = five); it should output the probability of winning via randomized
estimation involving one million games (n.b., one million games,
not one million rolls of the dice).
Make sure that your analyses and your program agree with one another!
*/

import java.util.Random;

public class DiceSolver {

  private static int[] roll (int numOfDice) {
    int[] roll = new int[numOfDice];
    Random r = new Random();
    for (int i = 0; i < numOfDice; i++) {
      roll[i] = r.nextInt(6) + 1;
    }
    return roll;
  }

  private static int countSixes(int[] roll) {
    int numOfSixes = 0;
    for (int i = 0; i < roll.length; i++) {
      if (roll[i] == 6) {
        numOfSixes++;
      }
    }
    return numOfSixes;
  }

  private static double winCounter(int numOfDice) {
    int numOfGames = 0;
    int numOfWins = 0;
    int numOfSixes = 10;
    float result = 0;

    for (int x = numOfGames; x < 1000000; x++) {
      int[] roll = roll(numOfDice);
      numOfSixes = countSixes(roll);
      if (numOfSixes == 0) {
        numOfGames++;
      }
      if (numOfSixes == 1) {
        numOfGames++;
        numOfWins++;
      }
    }
    result = ((float)numOfWins/numOfGames);
    return result;
  }

  public static void main (String[] args) {
    int numOfDice = 0;
    //VALIDITY CHECKS
    if (args.length > 1) {
      throw new IllegalArgumentException();
    }
    if (args.length > 0) {
      numOfDice = Integer.parseInt(args[0]);
      if (numOfDice == 3 || numOfDice == 5 || numOfDice == 8) {
        numOfDice = Integer.parseInt(args[0]);
      } else {
        throw new IllegalArgumentException();
      }
    } else {
      numOfDice = 5;
    }
    System.out.println(winCounter(numOfDice));
  }
}
