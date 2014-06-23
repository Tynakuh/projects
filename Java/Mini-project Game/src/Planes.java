package game;

import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;

public abstract class Planes 
{
	protected int x = 0,y = 0,face;
	protected Image[] image = new Image[4];
	protected boolean alive = true;
	
	public abstract void shoot();
	
	public abstract void gethit();
	
	public abstract void move(int xChange, int yChange);
	
	public void paint(Graphics2D g)
	{
		g.drawImage(image[face],x,-y,null);
	}
	
	public void setFace(int f)
	{
		if(f==0||f==1||f==2)
		face = f;
	}
	
	public int getX()
	{
		return x;
	}
	
	public int getY()
	{
		return y;
	}
	
	public Rectangle getCollision() 
	{
		return new Rectangle(x,y,24,35);
	}
	
	public boolean getAlive()
	{
		return alive;
	}
}
