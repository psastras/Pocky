package pineapple.main;

import pineapple.libs.PineappleLib;
import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public abstract class PineappleActivity extends Activity {

	static {
		System.loadLibrary("pineapple");
	}

	protected abstract GLSurfaceView.Renderer createRenderer();

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(new PineappleGLSurface(getApplicationContext(),
				createRenderer()));
		PineappleLib.init();
		PineappleLib.start();
	}

	@Override
	protected void onPause() {
		PineappleLib.stop();
		super.onPause();
	}

	@Override
	protected void onResume() {
		// PineappleLib.start();
		super.onResume();
	}

}