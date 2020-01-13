//
// Created by 김현규 on 2020/01/12.
//

#ifndef RFZF_VIEW_H
#define RFZF_VIEW_H

#include <string>
#include <vector>

class View {
public:
    View();

    ~View();

    void Print(std::vector<std::wstring> &paths);

    void Run();

private:
    void notify(std::function<void(std::string)> callback);
};

#endif //RFZF_VIEW_H
