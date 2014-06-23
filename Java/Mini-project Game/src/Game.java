package game;
import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.image.BufferStrategy;
import java.util.Vector;

import javax.swing.Timer;

public class Game extends Canvas implements KeyListener, ActionListener
{
    private boolean runFlag = false; //Determines if program should continue running
    private BufferStrategy strategy; //BufferStrategy for displaying paint
    private Background back = new Background(); //Creates background
    private Planes player = new Player(); //Creates player
    private Vector<Enemy> enemy = new Vector<Enemy>(0); //Holds enemy(s) currently present
    private Vector<Explosion> explosions = new Vector<Explosion>(0); //Holds current explosions (destroyed enemies)
    private boolean[] mFlag = {false,false,false,false}; //Holds flags for which keys are held down (WASD)
    private boolean aFlag = true; //Flag to allow for attacks to occur.
    private Timer attacktimer = new Timer(500,this); //Timer for attack cool down
    private Timer enemytimer = new Timer(500,this); //Timer for enemy attacks
    private Frame f; //Frame holding game
    private boolean loseFlag = false; //Determines if lost game.
    
    public static void main(String[] param)
    {
    	new Game().run(40);
    }
 
    public void run(double fps)
    { 
        runFlag = true;
 
        startup(); //Initiates frames, values etc.
        double next = (double)System.nanoTime() / 1000000000.0; //Gets time converting it to seconds; next represents next time we should run the loop.
        while(runFlag) //Start of game loop
        {
            double curr = (double)System.nanoTime() / 1000000000.0; //Gets time converting it to seconds; curr represents the current time, comparing it to next time to run loop.
            if(curr >= next)
            {
                next += (1.0/fps); //Adds delay until next loop to next
                update(); //Logic updates
                draw(); //Graphical updates
            }
            else //Else sleep until time till next has passed.
            {
                int sleep = (int)(1000.0 * (next - curr));
                if(sleep > 0)
                {
                    try
                    {
                        Thread.sleep(sleep);
                    }
                    catch(InterruptedException e)
                    {
                    }
                }
            }
        } //END of loop.
        shutdown(); //shutdown the game.
    }
 
    public void stop()
    {
    	loseFlag = true;
    	attacktimer.setInitialDelay(5000);
    	attacktimer.start();
    }
 
    public void startup() 
    {
        
        f = new Frame("Game");
        f.setLayout(new BorderLayout());
        f.addKeyListener(this);
        f.add(this);
        f.setSize(800,600);
        f.setResizable(false);
        f.setVisible(true);
        f.setFocusable(true);
        Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
        f.setLocation((dim.width-800)/2, (dim.height-600)/2);
        f.addWindowListener ( new WindowAdapter () {
        public void windowClosing ( WindowEvent evt )
        {
           System.exit(0);
        }
        });
        createBufferStrategy(2);
        strategy = getBufferStrategy();
        f.requestFocus();
        ((Player) player).intialise();
        enemytimer.start();
	}
    
    public void shutdown() 
    {
    	System.exit(0);
	}
    
    public void update() 
    {
        f.requestFocus();
    	checkenemies();
    	movement();
		killbullets();
		killplanes();
		if(!loseFlag)
		{
		checkplayercollisions();
		checkenemycollisions();
		}
	}
    
    public void draw() 
    {
    	Graphics2D g = (Graphics2D) strategy.getDrawGraphics();
        back.paint(g);
        if(!loseFlag)
        {
        	player.paint(g);
        	g.drawString("Lives: "+((Player) player).getLives(),700,500);
        }
        drawbullets(g);
        drawenemies(g);
        g.drawString("Lives: "+((Player) player).getLives(),700,500);
        if(loseFlag)
        {
        	g.setFont((new Font("Arial", 1 , 64)));
        	g.drawString("Game Over", 200,300);
        }
        strategy.show();
	}
    
    private void movement()
    {
    	if(mFlag[0])
    	{
    		player.move(0,4);
    	}
    	else if(mFlag[1])
    	{
    		player.move(0,-4);
    	}
    	if(mFlag[2])
    	{
    		player.setFace(1);
    		player.move(-4,0);
    	}
    	else if(mFlag[3])
    	{
    		player.move(4,0);
    		player.setFace(2);
    	}
    }
    
    private void attack()
    {
    	if(aFlag)
    	{
    		player.shoot();
    		aFlag = false;
    		attacktimer.setInitialDelay(500);
    		attacktimer.start();
    	}
    }

	public void keyPressed(KeyEvent e) 
	{
		if(e.getKeyChar() == 'w')
		{
			mFlag[0] = true;
		}
		if(e.getKeyChar() == 's')
		{
			mFlag[1] = true;
		}
		if(e.getKeyChar() == 'a')
		{
			mFlag[2] = true;
		}
		
		if(e.getKeyChar() == 'd')
		{
			mFlag[3] = true;
		}
		
		if(e.getKeyCode() == KeyEvent.VK_SPACE)
		{
			attack();
		}
	}

	public void keyReleased(KeyEvent e) 
	{
		if(e.getKeyChar() == 'w')
		{
			mFlag[0] = false;
		}
		if(e.getKeyChar() == 's')
		{
			mFlag[1] = false;
		}
		if(e.getKeyChar() == 'a')
		{
			mFlag[2] = false;
			player.setFace(0);
		}
		if(e.getKeyChar() == 'd')
		{
			mFlag[3] = false;
			player.setFace(0);
		}
	}

	public void keyTyped(KeyEvent e) 
	{
		//Required method. Unused.
	}

	public void actionPerformed(ActionEvent e) 
	{
		if(e.getSource() == attacktimer)
		{
			aFlag = true;
			attacktimer.stop();
			if(loseFlag)
				runFlag = false;
		}
		
		if(e.getSource() == enemytimer)
		{
			explosions.removeAllElements();
			for(int i =0; i<enemy.size(); i++)
			{
				enemy.get(i).shoot();
			}
		}
	}

	private void drawbullets(Graphics2D g)
	{
		Vector<PlayerBullet> playerbullets = ((Player) player).getBullets();
		for(int i = 0; i<playerbullets.size();i++)
		{
			playerbullets.get(i).paint(g);
		}
		for(int i = 0; i<enemy.size();i++)
		{
			Vector<EnemyBullet> eb = enemy.get(i).getBullets();
			for(int a = 0; a<eb.size(); a++)
			{
					eb.get(a).paint(g);
			}
		}
	}

	private void killbullets()
	{
		Vector<PlayerBullet> playerbullets = ((Player) player).getBullets();
		for(int i = 0; i<playerbullets.size();i++)
		{
			if(!(playerbullets.get(i).checkAlive()))
			{
				((Player) player).removeBullet(i);
			}
		}
		
		for(int i = 0; i<enemy.size();i++)
		{
			Vector<EnemyBullet> eb = enemy.get(i).getBullets();
			for(int a = 0; a<eb.size(); a++)
			{
				if(!(eb.get(a).checkAlive()))
				{
					enemy.get(i).removeBullet(a);
				}
			}
		}
	}

	private void killplanes()
	{
		if(!player.getAlive())
		{
			stop();
		}

		
		for(int i = 0; i<enemy.size();i++)
		{
				if(!(enemy.get(i).getAlive()))
				{
					explosions.add(new Explosion(enemy.get(i).getX(),enemy.get(i).getY()));
					enemy.remove(i);
				}
			}
		}
	
	private void drawenemies(Graphics2D g)
	{
		for(int i =0; i<enemy.size(); i++)
		{
			enemy.get(i).paint(g);
			if(player.getX()-enemy.get(i).getX()>0)
			{
				enemy.get(i).move(1,0);
				enemy.get(i).setFace(2);
			}
			else if(player.getX()-enemy.get(i).getX()<0)
			{
				enemy.get(i).move(-1,0);
				enemy.get(i).setFace(1);
			}
			else
				enemy.get(i).setFace(0);
			if(enemy.get(i).getY()>-50)
				enemy.get(i).move(0, -1);
		}
		
		for(int i = 0; i<explosions.size();i++)
		{
			explosions.get(i).paint(g);
		}
	}
	
	private void checkenemies()
	{
		int currenty = back.getY();
		if(currenty==-600)
		{
			enemy.add(new Enemy((int) (Math.random()*760),50));
		}
		if(currenty==-400)
		{
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
		}
		if(currenty==-200)
		{
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
		}
		if(currenty==-000)
		{
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
			enemy.add(new Enemy((int) (Math.random()*760),50));
		}
	}

	private void checkplayercollisions()
	{
		Rectangle pc = player.getCollision();
		for(int i = 0; i<enemy.size();i++)
		{
			Vector<EnemyBullet> eb = enemy.get(i).getBullets();
			for(int a = 0; a<eb.size(); a++)
			{
				if(pc.intersects(eb.get(a).getCollision()))
				{
					player.gethit();
					enemy.get(i).removeBullet(a);
				}
			}
		}
	}

	private void checkenemycollisions()
	{
		Vector<PlayerBullet> playerbullets = ((Player) player).getBullets();
		for(int i = 0; i<enemy.size();i++)
		{
			for(int a =0; a<playerbullets.size();a++)
			{
				if(playerbullets.get(a).getCollision().intersects(enemy.get(i).getCollision()))
				{
					enemy.get(i).gethit();
					((Player) player).removeBullet(a);
				}
			}
		}
	}
	
}
