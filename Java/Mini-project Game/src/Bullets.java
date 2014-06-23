package game;

import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;

public abstract class Bullets 
{
	protected int x,y,face;
	protected Image image;
	protected boolean alive = true;
	
	public abstract void move();
	
	public void paint(Graphics2D g)
	{
		g.drawImage(image,x,-y,null);
		move();
	}
	
	public Rectangle getCollision() 
	{
		return new Rectangle(x,y,20,20);
	}
	
	public boolean checkAlive()
	{
		return alive;
	}
}
