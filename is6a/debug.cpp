#include "is.h"
#include "random.h"
#include "timer.h"
#include "image.h"
#include "pngio.h"

#include <vector>
#include <random>

static constexpr float MINDIFF = 0.001f;

static void colours(ppc::random& rng, float a[3], float b[3]) {
    std::uniform_real_distribution<double> unif(0.0f, 1.0f);
    std::bernoulli_distribution coin(0.2);

    float maxdiff = 0;
    do {
        bool done = false;
        while (!done) {
            for (int k = 0; k < 3; ++k) {
                a[k] = unif(rng);
                b[k] = coin(rng) ? unif(rng) : a[k];
                if (a[k] != b[k]) {
                    done = true;
                }
            }
        }
        maxdiff = std::max({
                                   std::abs(a[0]-b[0]),
                                   std::abs(a[1]-b[1]),
                                   std::abs(a[2]-b[2])
                           });
    } while (maxdiff < MINDIFF);
}

static void dump(const float (&a)[3]) {
    std::cout << a[0] << "," << a[1] << "," << a[2];
}

static void dump(const Result& r) {
    std::cout << "  y0 = " << r.y0 << "\n";
    std::cout << "  x0 = " << r.x0 << "\n";
    std::cout << "  y1 = " << r.y1 << "\n";
    std::cout << "  x1 = " << r.x1 << "\n";
    std::cout << "  outer = "; dump(r.outer); std::cout << "\n";
    std::cout << "  inner = "; dump(r.inner); std::cout << "\n";
}

static bool close(float a, float b) {
    return std::abs(a - b) < 0.0001;
}

static bool equal(const float (&a)[3], const float (&b)[3]) {
    return close(a[0], b[0])
           && close(a[1], b[1])
           && close(a[2], b[2]);
}



static void test(ppc::random& rng, int ny, int nx, int y0, int x0,
                 int y1, int x1, bool binary, bool randomColors)
{
    Result e;
    e.y0 = y0;
    e.x0 = x0;
    e.y1 = y1;
    e.x1 = x1;
    if (binary) {
        for (int c = 0; c < 3; ++c) {
            e.inner[c] = 1.0f;
            e.outer[c] = 0.0f;
        }
    }
    else {
        if (randomColors) {
            // Random but distinct colours
            colours(rng, e.inner, e.outer);
        }
            // Test worst-case scenario
        else {
            for (int c = 0; c < 3; ++c) {
                e.inner[c] = 1.0f;
                e.outer[c] = 1.0f;
            }
            e.outer[0] -= MINDIFF;
        }
    }


    std::vector<float> data(3*ny*nx);
    for (int y = 0; y < ny; ++y) {
        for (int x = 0; x < nx; ++x) {
            for (int c = 0; c < 3; ++c) {
                bool inside = y0 <= y && y < y1 && x0 <= x && x < x1;
                data[c + 3 * x + 3 * nx * y] = inside ? e.inner[c] : e.outer[c];
            }
        }
    }

    Result r;
    std::cout << "is\t" << ny << '\t' << nx << '\t' << binary << '\t' << randomColors << '\t' << std::flush;
    {
        ppc::timer t;
        r = segment(ny, nx, data.data());
    }
    std::cout << std::endl;
}

static void test_single(ppc::random& rng, int ny, int nx, bool binary, bool random_only = false)
{
    if (ny*nx <= 2) return;

    bool ok = false;
    int y0, x0, y1, x1;
    do {
        // Random box location
        std::uniform_int_distribution<int> dy0(0, ny-1);
        std::uniform_int_distribution<int> dx0(0, nx-1);
        y0 = dy0(rng);
        x0 = dx0(rng);

        std::uniform_int_distribution<int> dy1(y0+1, ny);
        std::uniform_int_distribution<int> dx1(x0+1, nx);
        y1 = dy1(rng);
        x1 = dx1(rng);
        // Avoid ambiguous cases
        if      (y0 == 0 && y1 == ny && x0 == 0)  { ok = false; }
        else if (y0 == 0 && y1 == ny && x1 == nx) { ok = false; }
        else if (x0 == 0 && x1 == nx && y0 == 0)  { ok = false; }
        else if (x0 == 0 && x1 == nx && y1 == ny) { ok = false; }
        else { ok = true; }
    } while (!ok);

    test(rng, ny, nx, y0, x0, y1, x1, binary, true);
}

static void test(ppc::random& rng, int ny, int nx, bool binary) {
    for (int i = 0; i < 10; ++i) {
        test_single(rng, ny, nx, binary);
    }
}

static std::vector<float> generate_gradient(
        int ny, int nx, int x0, int x1, int y0, int y1, int y2,
        float color_outer, float color_inner)
{
    std::vector<float> bitmap(nx*ny*3);
    const float fact = 1.0f / float(y2-y1);

    for (int y = 0; y < ny; ++y) {
        const bool yinside = y >= y0 && y < y1;
        const bool yinside_gradient = y >= y1 && y < y2;
        for (int x = 0; x < nx; ++x) {
            const auto pixel_base = (nx*y + x)*3;
            const bool xinside = x >= x0 && x < x1;
            const bool inside = yinside && xinside;
            const bool inside_gradient = yinside_gradient && xinside;

            if (inside) {
                for (int c = 0; c < 3; ++c) {
                    bitmap[pixel_base+c] = color_inner;
                }
            }
            else if (inside_gradient) {
                const float val = float(y2-y) * fact * (color_inner-color_outer) + color_outer;
                for (int c = 0; c < 3; ++c) {
                    bitmap[pixel_base+c] = val;
                }
            }
            else {
                for (int c = 0; c < 3; ++c) {
                    bitmap[pixel_base+c] = color_outer;
                }
            }
        }
    }
    return bitmap;
}


static void test_gradient(ppc::random& rng,
                          int ny, int nx, int x0, int x1,
                          int y0, int y1, int y2)
{
    float color_outer;
    float color_inner;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    do {
        color_outer = dist(rng);
        color_inner = dist(rng);
    } while(std::abs(color_outer-color_inner) < MINDIFF);

    const auto data = generate_gradient(
            ny, nx, x0, x1, y0, y1, y2, color_outer, color_inner
    );

    Result r;
    std::cout << "is\t" << ny << "\t" << nx << "\t" << 2 << '\t' << 0 << '\t' << std::flush;
    {
        ppc::timer t;
        r = segment(ny, nx, data.data());
    }
    std::cout << '\n';
}

static void test_gradient(ppc::random& rng, int ny, int nx)
{
    for (int i = 0; i < 10; ++i) {
        // Random box location
        std::uniform_int_distribution<int> dx0(0, nx-1);  int x0 = dx0(rng);
        std::uniform_int_distribution<int> dx1(x0+1, nx); int x1 = dx1(rng);
        std::uniform_int_distribution<int> dy0(0, ny-1);  int y0 = dy0(rng);
        std::uniform_int_distribution<int> dy1(y0+1, ny); int y1 = dy1(rng);
        std::uniform_int_distribution<int> dy2(y1, ny);   int y2 = dy2(rng);

        // Avoid ambiguous cases
        if (!(x0 > 0 && x1 < nx && y0 > 0 && y2 < ny)) continue;
        test_gradient(rng, ny, nx, x0, x1, y0, y1, y2);
    }
}

static void do_default_test(ppc::random& rng, bool binary) {
    for (int ny = 1; ny < 60; ++ny) {
        test(rng, ny, 1, binary);
        test(rng, 1, ny, binary);
        if (!binary) {
            test_gradient(rng, ny, 1);
        }
    }
    for (int ny = 2; ny < 60; ny += 13) {
        for (int nx = 2; nx < 60; nx += 7) {
            test(rng, ny, nx, binary);
            if (!binary) {
                test_gradient(rng, ny, nx);
            }
        }

    }
    test(rng, 1000, 1, binary);
    test(rng, 1, 1000, binary);
    test(rng, 1000, 2, binary);
    test(rng, 2, 1000, binary);
    test(rng, 100, 50, binary);
}