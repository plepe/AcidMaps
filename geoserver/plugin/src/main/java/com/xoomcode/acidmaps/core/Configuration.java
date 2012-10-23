package com.xoomcode.acidmaps.core;

// TODO: Auto-generated Javadoc
/**
 * @date 09/11/2010
 * @author cfarina
 * Configuration is used to send AMS parameters to the C++ AMS library.
 */
public class Configuration {

	/** Set of valued points, stored 3 floats per point. X, Y, Value */
	public Point[] dataset;
	
	public int datasetSize;

	/** Desired simplify method. */
	public int simplifyMethod;

	/** Size of the simplified sample. */
	public int simplifySize;

	/** Requested tile bounds. */
	public Bounds bounds;

	/** Strategy used to interpolate missing values. */
	public int interpolationStrategy;

	/** Output image width. */
	public int width;

	/** Output image height. */
	public int height;

	/** Radius for every interpolation type. */
	public int radius;
	
	/** How to calculate radius */
	public int radiusMethod;

        /** Zoom Factor for Radius (needs RADIUS_METHOD=1) */
	public float radiusZoomFactor;

	/** How to calculate the weight depending on the distance of each points */
	public int distanceMethod;

	public float[] intervals;
	
	public Color[] intervalsColors;
	
	public int intervalsSize;
	
	public int rendererType;
	
	public int format;
	
	public float measureQuantil;

	public int quantilMethod;

	public float quantilOffset;

	public int weightMethod;

	public float[] weights;

	public float[] weightsValues;

	/// The zoom factor (pixels per data units)
	public float zoom;

	public int error;

	/**
	 * Instantiates a new configuration.
	 */
	public Configuration() {
	}
}
