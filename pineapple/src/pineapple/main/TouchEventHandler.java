package pineapple.main;

import android.view.MotionEvent;

public interface TouchEventHandler {

	public abstract void onTouch(MotionEvent event);

	public abstract void onDrag(MotionEvent event);

	public abstract void onRelease(MotionEvent event);
}
