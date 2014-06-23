package game;

import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class PlayerBullet extends Bullets {
	public PlayerBullet(int initX, int initY)
	{
        try {
			image = ImageIO.read(new File("images/bullet/1.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
        x = initX+2;
        y = initY;
	}
	
	public void move() 
	{
		y += 10;
		if(y>0)
		{
			alive = false;
		}
	}

}
