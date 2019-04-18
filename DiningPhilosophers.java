//code based on https://github.com/rupakraj/dining-philosopher/blob/master/Dine.java

/*  This program is immune to deadlock through resource ordering. Assuming each
fork is numbered from 0 - 4, the program ensures that each philosopher looks
at the availability of the lowest numbered fork first. If that fork is not
available, the philosopher then drops the low numbered fork, waits, and tries
again. There will be no deadlock because there will be no instance that every
philosopher will be holding one fork because each philosopher will check the
same one as one who is sitting next to them.
*/

import java.util.Random;
public class DiningPhilosophers{

	public static void main(String[] args){

		int rounds = 10;

		Fork[] fork = new Fork[5];

		for(int i = 0; i < fork.length; i++){
			fork[i] = new Fork("C: " + i);
		}
		Philosopher[] philosophers = new Philosopher[5];

		philosophers[0] = new Philosopher("P: 0 - ", fork[0], fork[1], rounds);
		philosophers[1] = new Philosopher("P: 1 - ", fork[1], fork[2], rounds);
		philosophers[2] = new Philosopher("P: 2 - ", fork[2], fork[3], rounds);
		philosophers[3] = new Philosopher("P: 3 - ", fork[3], fork[4], rounds);
		philosophers[4] = new Philosopher("P: 4 - ", fork[0], fork[4], rounds);

		for(int i = 0; i < philosophers.length; i++){
			System.out.println("Thread " + i + " has started");
			Thread t = new Thread(philosophers[i]);
			t.start();
		}
 	}
}

// When state == 1, pause
class Philosopher extends Thread {
	private Fork leftFork;
	private Fork rightFork;

	private String name;
	private int state;
	private int rounds;

	public Philosopher ( String name, Fork left, Fork right, int rounds){
		this.state = 1;
		this.name = name;
		leftFork = left;
		rightFork = right;
		rounds = rounds;
	}

	public void eat() {
		if( ! leftFork.used ){
			if( !rightFork.used ){

				leftFork.take();
				rightFork.take();
				System.out.println(name + " : Eat");
        try{
    			Thread.sleep(1000);
    		}
    		catch(InterruptedException ex){ }

				rightFork.drop();
		 		leftFork.drop();

			} leftFork.drop();
        pause();
		}
		pause();
	}

	public void pause(){
      Random rand = new Random();
      int randInt = rand.nextInt(1000);
		 	this.state = 1;
		 	System.out.println(name + " : pause");
      try{
        Thread.sleep(randInt);
      }
      catch(InterruptedException ex){ }
	}

	public void run(){
		for(int i = 0; i <= rounds; i++){
			eat();
		}
	}
}

class Fork{

	public boolean used;
	public String name;

	public Fork(String name){
		this.name = name;
	}

	public synchronized void take() {
		System.out.println ("Used :: " + name );
		this.used = true;
	}
	public synchronized void drop() {
		System.out.println ("dropped :: " + name );
		this.used = false ;
	}
}
