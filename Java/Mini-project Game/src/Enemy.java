package game;

import java.io.File;
import java.io.IOException;
import java.util.Vector;

import javax.imageio.ImageIO;

public class Enemy extends Planes  {
	
	private Vector<EnemyBullet> bullets = new Vector<EnemyBullet>(0);
	public Enemy(int initX, int initY)
	{
		x = initX;
		y = initY;
        try {
			image[0] = ImageIO.read(new File("images/enemy/1.png"));
			image[1] = ImageIO.read(new File("images/enemy/2.png"));
			image[2] = ImageIO.read(new File("images/enemy/3.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void shoot() 
	{
		bullets.add(new EnemyBullet(x,y));
	}
	
	public void gethit()
	{
		alive = false;
	}

	public void move(int xChange, int yChange) 
	{
			x += xChange;
			y += yChange;
	}
	
	public Vector<EnemyBullet> getBullets()
	{
		return bullets;
	}

	public void removeBullet(int i)
	{
		bullets.remove(i);
	}
}
