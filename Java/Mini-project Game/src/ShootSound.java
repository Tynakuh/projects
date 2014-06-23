package game;

import java.io.File;

import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.DataLine;

public class ShootSound 
{
	public ShootSound()
	{
		try {
			AudioInputStream audioInputStream;
			File file = new File("sounds/shoot.wav");
			audioInputStream = AudioSystem.getAudioInputStream(file);
			Clip line;
			line = (Clip) AudioSystem.getLine(new DataLine.Info(Clip.class, audioInputStream.getFormat()));
			line.open(audioInputStream);
			line.start();
			line.drain();
			audioInputStream.close();
			}
			catch (Exception e)
			{
			e.printStackTrace();
			}
	}
}
