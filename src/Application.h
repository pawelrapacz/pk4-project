#pragma once

namespace Battleships {

    struct ApplicationSpecs {
        int width         = 400;
        int height        = width / 16 * 9;
        int scale         = 3;
        const char* title = "Battleships";
    };

    class Application {
    public:
        Application(const ApplicationSpecs& specs = ApplicationSpecs());
        ~Application() = default;

        void Run();
        void UpdateDrawFrame();

    private:
        ApplicationSpecs _specs;
    };

}  // namespace Battleships