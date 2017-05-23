//
// MATLAB Compiler: 6.0 (R2015a)
// Date: Tue May 16 13:30:08 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:SCSRv2" "-T" "link:lib" "-d"
// "C:\Matlab_data\SCSRv2\for_testing" "-v" "C:\Matlab_data\CompareHOG.m"
// "C:\Matlab_data\FillToSquare.m" "C:\Matlab_data\FinalPredict.m"
// "C:\Matlab_data\HOG.m" "C:\Matlab_data\netFcn.m"
// "C:\Matlab_data\ProportionalResize.m" "C:\Matlab_data\RemoveWhiteSpace.m"
// "C:\Matlab_data\ThickenLines.m" "C:\Matlab_data\ThickenLines_core.m" "-a"
// "C:\Matlab_data\orginal_hog.mat" 
//

#ifndef __SCSRv2_h
#define __SCSRv2_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_SCSRv2
#define PUBLIC_SCSRv2_C_API __global
#else
#define PUBLIC_SCSRv2_C_API /* No import statement needed. */
#endif

#define LIB_SCSRv2_C_API PUBLIC_SCSRv2_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_SCSRv2
#define PUBLIC_SCSRv2_C_API __declspec(dllexport)
#else
#define PUBLIC_SCSRv2_C_API __declspec(dllimport)
#endif

#define LIB_SCSRv2_C_API PUBLIC_SCSRv2_C_API


#else

#define LIB_SCSRv2_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_SCSRv2_C_API 
#define LIB_SCSRv2_C_API /* No special import/export declaration */
#endif

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV SCSRv2InitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV SCSRv2Initialize(void);

extern LIB_SCSRv2_C_API 
void MW_CALL_CONV SCSRv2Terminate(void);



extern LIB_SCSRv2_C_API 
void MW_CALL_CONV SCSRv2PrintStackTrace(void);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxCompareHOG(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxFillToSquare(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxFinalPredict(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxHOG(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxNetFcn(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxProportionalResize(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                        *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxRemoveWhiteSpace(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                      *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxThickenLines(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_SCSRv2_C_API 
bool MW_CALL_CONV mlxThickenLines_core(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                       *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_SCSRv2
#define PUBLIC_SCSRv2_CPP_API __declspec(dllexport)
#else
#define PUBLIC_SCSRv2_CPP_API __declspec(dllimport)
#endif

#define LIB_SCSRv2_CPP_API PUBLIC_SCSRv2_CPP_API

#else

#if !defined(LIB_SCSRv2_CPP_API)
#if defined(LIB_SCSRv2_C_API)
#define LIB_SCSRv2_CPP_API LIB_SCSRv2_C_API
#else
#define LIB_SCSRv2_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV CompareHOG(int nargout, mwArray& CompareHOG, const mwArray& hog1, const mwArray& hog2);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV FillToSquare(int nargout, mwArray& output, const mwArray& input);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV FinalPredict(int nargout, mwArray& result, const mwArray& picturepath);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV HOG(int nargout, mwArray& H, const mwArray& Im);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV netFcn(int nargout, mwArray& Y, mwArray& Xf, mwArray& Af, const mwArray& X, const mwArray& _U4b, const mwArray& _U4c);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV ProportionalResize(int nargout, mwArray& output, mwArray& scale, const mwArray& input);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV RemoveWhiteSpace(int nargout, mwArray& output, const mwArray& input);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV ThickenLines(int nargout, mwArray& output, const mwArray& input, const mwArray& scale);

extern LIB_SCSRv2_CPP_API void MW_CALL_CONV ThickenLines_core(int nargout, mwArray& output, const mwArray& input, const mwArray& brush_matrix);

#endif
#endif
