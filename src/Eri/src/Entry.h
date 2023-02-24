#pragma once

extern ERI::App *ERI::CreateApp();

int main(int argc, char **argv)
{
    auto app = ERI::CreateApp();

    app->Run();
    delete app;

}