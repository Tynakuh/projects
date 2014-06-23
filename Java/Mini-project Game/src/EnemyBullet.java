package game;

import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class EnemyBullet extends Bullets {

	public EnemyBullet(int initX, int initY)
	{
        try {
			image = ImageIO.read(new File("images/bullet/2.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
        x = initX+2;
        y = initY;
	}
	
	public void move() 
	{
		y -= 10;
		if(y<-600)
		{
			alive = false;
		}
	}
}
