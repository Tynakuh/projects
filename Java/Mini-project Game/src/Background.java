package game;

import java.awt.Graphics2D;
import java.awt.Image;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class Background 
{
	private Image image;
	private double yoffset = 0;
	public Background()
	{
        try {
			image = ImageIO.read(new File("images/background.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void paint(Graphics2D g)
	{
		g.drawImage(image, 0,-625+(int) yoffset,null);
		if(yoffset<625)
		yoffset = yoffset+(1.0/3.0);
	}
	
	public int getY()
	{
		return -625+(int) yoffset;
	}
}
