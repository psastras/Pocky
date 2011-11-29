package pocky.main;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import pineapple.main.PineappleActivity;
import pocky.libs.Pocky;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class PockyActivity extends PineappleActivity {

	static {
		System.loadLibrary("pocky");
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Pocky.startup();

	}

	@Override
	protected GLSurfaceView.Renderer createRenderer() {
		return new PockyRenderer();
	}

	private static class PockyRenderer implements GLSurfaceView.Renderer {
		public void onDrawFrame(GL10 gl) {
			Pocky.draw();
		}

		public void onSurfaceChanged(GL10 gl, int width, int height) {
			Pocky.resize(width, height);
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {

		}
	}
}