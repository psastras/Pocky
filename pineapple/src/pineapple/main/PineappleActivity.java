package pineapple.main;

import pineapple.libs.PineappleLib;
import android.app.Activity;
import android.os.Bundle;

public class PineappleActivity extends Activity {

	static {
		System.loadLibrary("pineapple");
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		PineappleLib.init();
		PineappleLib.test();
	}

}