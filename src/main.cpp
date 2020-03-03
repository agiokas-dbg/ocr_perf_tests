#include <string>
#include <memory>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

#include "threadible.hpp"
#include <chrono>

using namespace std::chrono_literals;
using namespace std;
using namespace cv;


struct task
{
    cv::Mat image;
    task(cv::Mat arg) : image(arg) {}
};

struct resource
{
    int num;
    std::unique_ptr<tesseract::TessBaseAPI> ocr;
    std::mutex mutex;

    resource(int i) : num(i)
    {
        ocr = std::make_unique<tesseract::TessBaseAPI>(tesseract::TessBaseAPI());
        ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
        ocr->SetPageSegMode(tesseract::PSM_AUTO);
    }

    /// @brief do the actual work here
    void print(task & t) const
    { 
        auto im = t.image;
        ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);
        auto outText = string(ocr->GetUTF8Text());
        std::cout << outText << std::endl;
    }

    /// @brief expose this operator which will be called by `threadible::submit`
    template <typename... Args>
    void operator()(Args... args) const
    {
        print(args...);
    }

    ~resource()
    {
        ocr->End();
    }
};


auto main(int argc, char* argv[]) -> int
{
    string outText;
    string imPath = argv[1];
    cv::Mat im = cv::imread(imPath, IMREAD_COLOR);

    threadible<resource> tpool(10);
    for (int i = 0; i < 100; i++) {
        tpool.submit(task(im));
    }
    tpool.stop();
    return 0;
}
