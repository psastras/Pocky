package pocky.main;

import pineapple.main.PineappleActivity;
import android.os.Bundle;

public class PockyActivity extends PineappleActivity {
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
	}
}