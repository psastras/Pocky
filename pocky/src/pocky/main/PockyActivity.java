package pocky.main;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import pineapple.main.PineappleActivity;
import pocky.libs.Pocky;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
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

	private class PockyRenderer implements GLSurfaceView.Renderer {
		public void onDrawFrame(GL10 gl) {
			Pocky.draw((int) elapsedTimeMillis());
		}

		public void onSurfaceChanged(GL10 gl, int width, int height) {
			Pocky.resize(width, height);
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {

		}
	}

	@Override
	protected String getAPKPath() {
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = getPackageManager();
		try {
			appInfo = packMgmr.getApplicationInfo("pocky.main", 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		apkFilePath = appInfo.sourceDir;
		return apkFilePath;
	}
}