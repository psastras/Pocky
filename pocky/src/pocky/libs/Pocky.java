package pocky.libs;

public class Pocky {
	/**
	 * Startup must be called before using any other methods (except for
	 * shutdown).
	 */
	public static native void startup();

	/**
	 * Shutdown frees all the resources allocated. Calling any other method
	 * except for startup() after shutdown() is not a good idea.
	 */
	public static native void shutdown();

	/**
	 * Resizes the gl viewport and corresponding framebuffers.
	 * 
	 * @param w
	 * @param h
	 */
	public static native void resize(int w, int h);

	/**
	 * Causes the current scene to be drawn.
	 */
	public static native void draw(int time);
}
