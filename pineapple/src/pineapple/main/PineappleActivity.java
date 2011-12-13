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

	protected abstract String getAPKPath();

	protected abstract TouchEventHandler getTouchHandler();

	protected long startTime_;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		startTime_ = System.currentTimeMillis();
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(new PineappleGLSurface(getApplicationContext(),
				createRenderer(), getTouchHandler()));
		WindowManager.LayoutParams lp = getWindow().getAttributes();
		lp.screenBrightness = 100 / 100.0f;
		getWindow().setAttributes(lp);
		PineappleLib.init(getAPKPath());
		PineappleLib.start();

	}

	@Override
	protected void onPause() {
		System.out.println("PAUZE");
		PineappleLib.stop();
		super.onPause();
	}

	@Override
	protected void onResume() {
		// PineappleLib.start();
		super.onResume();
	}

	protected long elapsedTimeMillis() {
		return System.currentTimeMillis() - startTime_;
	}

}