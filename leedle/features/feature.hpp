#pragma once

struct Feature {
    virtual void create_variables() = 0;
    virtual void save_variables() = 0;
    virtual void load_variables() = 0;
};