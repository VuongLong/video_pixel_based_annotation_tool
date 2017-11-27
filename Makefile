LIBS_OPENCV = `$(PREFIX)pkg-config --libs opencv`
INCLUDE_OPENCV = `$(PREFIX)pkg-config --cflags opencv`

default: 
	g++ -o anotation_tool -O3 $(INCLUDE_OPENCV) anotation_tool.cpp -L/usr/local/lib/ -lopencv_stitching.3.3.0 -lopencv_superres.3.3.0 -lopencv_videostab.3.3.0 -lopencv_photo.3.3.0 -lopencv_aruco.3.3.0 -lopencv_bgsegm.3.3.0 -lopencv_bioinspired.3.3.0 -lopencv_ccalib.3.3.0 -lopencv_dpm.3.3.0 -lopencv_face.3.3.0 -lopencv_fuzzy.3.3.0 -lopencv_img_hash.3.3.0 -lopencv_line_descriptor.3.3.0 -lopencv_optflow.3.3.0 -lopencv_reg.3.3.0 -lopencv_rgbd.3.3.0 -lopencv_saliency.3.3.0 -lopencv_stereo.3.3.0 -lopencv_structured_light.3.3.0 -lopencv_phase_unwrapping.3.3.0 -lopencv_surface_matching.3.3.0 -lopencv_tracking.3.3.0 -lopencv_datasets.3.3.0 -lopencv_text.3.3.0 -lopencv_dnn.3.3.0 -lopencv_plot.3.3.0 -lopencv_xfeatures2d.3.3.0 -lopencv_shape.3.3.0 -lopencv_video.3.3.0 -lopencv_ml.3.3.0 -lopencv_ximgproc.3.3.0 -lopencv_calib3d.3.3.0 -lopencv_features2d.3.3.0 -lopencv_highgui.3.3.0 -lopencv_videoio.3.3.0 -lopencv_flann.3.3.0 -lopencv_xobjdetect.3.3.0 -lopencv_imgcodecs.3.3.0 -lopencv_objdetect.3.3.0 -lopencv_xphoto.3.3.0 -lopencv_imgproc.3.3.0 -lopencv_core.3.3.0