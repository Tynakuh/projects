package game;

import java.io.File;
import java.io.IOException;
import java.util.Vector;

import javax.imageio.ImageIO;

public class Player extends Planes {
	private Vector<PlayerBullet> bullets = new Vector<PlayerBullet>(0);
	private int lives = 3;
	public Player()
	{
        try {
			image[0] = ImageIO.read(new File("images/player/1.png"));
			image[1] = ImageIO.read(new File("images/player/2.png"));
			image[2] = ImageIO.read(new File("images/player/3.png"));
			image[3] = ImageIO.read(new File("images/explosion/1.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
        x =375 ;
        y =-400 ;
	}
	public void intialise()
	{
        try {
			image[0] = ImageIO.read(new File("images/player/1.png"));
			image[1] = ImageIO.read(new File("images/player/2.png"));
			image[2] = ImageIO.read(new File("images/player/3.png"));
			image[3] = ImageIO.read(new File("images/explosion/1.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
        x =375 ;
        y =-400 ;
	}
	public void shoot() 
	{
		new ShootSound();
		bullets.add(new PlayerBullet(x,y));
	}
	
	public void gethit()
	{
		face = 3;
		lives--;
		new ExplosionSound();
		if(lives<=0)
		{
			alive = false;
		}
	}

	public void move(int xChange, int yChange) 
	{
		if(x>=0 && xChange<0 || xChange>0 && x<=760)
		{
			x += xChange;
		}
		if(y<=-300 && yChange>0 || yChange<0 && y>=-530)
		{
			y += yChange;
		}
	}
	
	public Vector<PlayerBullet> getBullets()
	{
		return bullets;
	}
	
	public void removeBullet(int i)
	{
		bullets.remove(i);
	}

	public int getLives()
	{
		return lives;
	}
	
}
