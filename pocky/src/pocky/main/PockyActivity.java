package pocky.main;

import pineapple.main.PineappleActivity;
import pocky.libs.Pocky;
import android.os.Bundle;

public class PockyActivity extends PineappleActivity {

	static {
		System.loadLibrary("pocky");
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		Pocky.startup();
	}
}