package game;

import java.awt.Graphics2D;
import java.awt.Image;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Explosion {
	Image image;
	private int x, y;
	
	public Explosion(int initX, int initY)
	{
	    new ExplosionSound();
		x = initX;
		y = initY;
		try {
			image = ImageIO.read(new File("images/explosion/1.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	public void paint(Graphics2D g)
	{
		g.drawImage(image,x,-y,null);
	}
}
