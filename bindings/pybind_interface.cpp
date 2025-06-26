// #include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
// #include "DBoW2/DBoW2.h"
// #include <opencv2/opencv.hpp>
// #include <stdexcept>

// namespace py = pybind11;

// class DBoW2Wrapper {
// public:
//     DBoW2Wrapper(const std::string& vocab_file) {
//         std::cout << "Trying to load: " << vocab_file << std::endl;

//         if (!voc.loadFromTextFile(vocab_file)) {
//             throw std::runtime_error("Failed to load ORB vocabulary from text file.");
//         }

//         std::cout << "Vocabulary loaded successfully in Python wrapper!" << std::endl;
//     }

//     void add(const std::vector<cv::Mat>& descriptors) {
//         db.add(descriptors);
//     }

//     void save(const std::string& path) {
//         db.save(path);
//     }

//     void load(const std::string& path) {
//         db.load(path);
//     }

//     void clear() {
//         db.clear();
//     }

//     int size() const {
//         return db.size();
//     }

//     std::vector<std::pair<int, float>> query(const std::vector<cv::Mat>& descriptors, int max_results = 4) {
//         DBoW2::QueryResults results;
//         db.query(descriptors, results, max_results);
//         std::vector<std::pair<int, float>> out;
//         for (const auto& r : results) {
//             out.emplace_back(r.Id, r.Score);
//         }
//         return out;
//     }

// private:
//     DBoW2::TemplatedVocabulary<DBoW2::FORB::TDescriptor, DBoW2::FORB> voc;
//     DBoW2::TemplatedDatabase<DBoW2::FORB::TDescriptor, DBoW2::FORB> db;
// };


// PYBIND11_MODULE(dbow2py, m) {
//     py::class_<DBoW2Wrapper>(m, "DBoW2Wrapper")
//         .def(py::init<const std::string&>())
//         .def("add", &DBoW2Wrapper::add)
//         .def("query", &DBoW2Wrapper::query);
// }


// #include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
// #include <pybind11/numpy.h>
// #include <opencv2/opencv.hpp>
// #include "DBoW2/DBoW2.h"

// namespace py = pybind11;

// using Descriptor = DBoW2::FORB::TDescriptor;
// using Vocabulary = DBoW2::TemplatedVocabulary<Descriptor, DBoW2::FORB>;
// using Database = DBoW2::TemplatedDatabase<Descriptor, DBoW2::FORB>;

// class DBoW2Wrapper {
// public:
//     DBoW2Wrapper(const std::string& vocab_file) {
//         if (!voc.loadFromTextFile(vocab_file)) {
//             throw std::runtime_error("Failed to load vocabulary from file.");
//         }
//     }

//     void add(const std::vector<py::array_t<uint8_t>>& descriptors) {
//         std::vector<cv::Mat> vec = to_cv_mat_list(descriptors);
//         db.add(vec);
//         stored_descriptors.push_back(vec);
//     }

//     std::vector<std::pair<int, float>> query(const std::vector<py::array_t<uint8_t>>& descriptors, int max_results = 4) {
//         std::vector<cv::Mat> vec = to_cv_mat_list(descriptors);
//         DBoW2::QueryResults results;
//         db.query(vec, results, max_results);

//         std::vector<std::pair<int, float>> out;
//         for (const auto& r : results) {
//             out.emplace_back(r.Id, r.Score);
//         }
//         return out;
//     }

//     void save(const std::string& filename) {
//         db.save(filename);
//     }

//     void load(const std::string& filename) {
//         db.load(filename);
//     }

//     void clear() {
//         db.clear();
//         stored_descriptors.clear();
//     }

//     int size() const {
//         return db.size();
//     }

//     double score(const std::vector<py::array_t<uint8_t>>& descriptors1,
//                  const std::vector<py::array_t<uint8_t>>& descriptors2) {
//         std::vector<cv::Mat> vec1 = to_cv_mat_list(descriptors1);
//         std::vector<cv::Mat> vec2 = to_cv_mat_list(descriptors2);

//         DBoW2::BowVector v1, v2;
//         voc.transform(vec1, v1);
//         voc.transform(vec2, v2);

//         return voc.score(v1, v2);
//     }

//     py::list get_bow_vector(const std::vector<py::array_t<uint8_t>>& descriptors) {
//         std::vector<cv::Mat> vec = to_cv_mat_list(descriptors);
//         DBoW2::BowVector v;
//         voc.transform(vec, v);

//         py::list bow_list;
//         for (auto& kv : v) {
//             bow_list.append(py::make_tuple(kv.first, kv.second));
//         }
//         return bow_list;
//     }

//     py::list get_all_descriptors() const {
//         py::list out;
//         for (const auto& frame : stored_descriptors) {
//             py::list desc_list;
//             for (const auto& desc : frame) {
//                 py::array_t<uint8_t> npy = py::array_t<uint8_t>({desc.rows, desc.cols}, desc.data);
//                 desc_list.append(npy);
//             }
//             out.append(desc_list);
//         }
//         return out;
//     }

// private:
//     Vocabulary voc;
//     Database db;
//     std::vector<std::vector<cv::Mat>> stored_descriptors;

//     std::vector<cv::Mat> to_cv_mat_list(const std::vector<py::array_t<uint8_t>>& numpy_descs) const {
//         std::vector<cv::Mat> mats;
//         for (const auto& array : numpy_descs) {
//             if (array.ndim() != 2 || array.shape(1) != 32) {
//                 throw std::runtime_error("Each descriptor must be a 2D numpy array with 32 columns (ORB).");
//             }
//             mats.emplace_back(cv::Mat(array.shape(0), 32, CV_8U, const_cast<uint8_t*>(array.data())).clone());
//         }
//         return mats;
//     }
// };

// PYBIND11_MODULE(dbow2py, m) {
//     py::class_<DBoW2Wrapper>(m, "DBoW2Wrapper")
//         .def(py::init<const std::string&>())
//         .def("add", &DBoW2Wrapper::add)
//         .def("query", &DBoW2Wrapper::query)
//         .def("save", &DBoW2Wrapper::save)
//         .def("load", &DBoW2Wrapper::load)
//         .def("clear", &DBoW2Wrapper::clear)
//         .def("size", &DBoW2Wrapper::size)
//         .def("score", &DBoW2Wrapper::score)
//         .def("get_bow_vector", &DBoW2Wrapper::get_bow_vector)
//         .def("get_all_descriptors", &DBoW2Wrapper::get_all_descriptors);
// }

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include "DBoW2/DBoW2.h"

namespace py = pybind11;

using Descriptor = DBoW2::FORB::TDescriptor;
using Vocabulary = DBoW2::TemplatedVocabulary<Descriptor, DBoW2::FORB>;
using Database = DBoW2::TemplatedDatabase<Descriptor, DBoW2::FORB>;

class DBoW2Wrapper {
public:
    DBoW2Wrapper(const std::string& vocab_file) {
        if (!voc.loadFromTextFile(vocab_file)) {
            throw std::runtime_error("Failed to load vocabulary from file.");
        }
    }

    void add(const std::vector<py::array_t<uint8_t>>& descriptors) {
        std::vector<cv::Mat> vec = to_cv_mat_list(descriptors);
        db.add(vec);
        stored_descriptors.push_back(vec);
    }

    void add(py::array_t<uint8_t> d) {
        add({d});
    }

    std::vector<std::pair<int, float>> query(const std::vector<py::array_t<uint8_t>>& descriptors, int max_results = 4) {
        std::vector<cv::Mat> vec = to_cv_mat_list(descriptors);
        DBoW2::QueryResults results;
        db.query(vec, results, max_results);

        std::vector<std::pair<int, float>> out;
        for (const auto& r : results) {
            out.emplace_back(r.Id, r.Score);
        }
        return out;
    }

    std::vector<std::pair<int, float>> query(py::array_t<uint8_t> desc, int max_results = 4) {
        return query({desc}, max_results);
    }

    void save(const std::string& filename) {
        db.save(filename);
    }

    void load(const std::string& filename) {
        db.load(filename);
    }

    void clear() {
        db.clear();
        stored_descriptors.clear();
    }

    int size() const {
        return db.size();
    }

    double score(const std::vector<py::array_t<uint8_t>>& descriptors1,
                 const std::vector<py::array_t<uint8_t>>& descriptors2) {
        std::vector<cv::Mat> vec1 = to_cv_mat_list(descriptors1);
        std::vector<cv::Mat> vec2 = to_cv_mat_list(descriptors2);

        DBoW2::BowVector v1, v2;
        voc.transform(vec1, v1);
        voc.transform(vec2, v2);

        return voc.score(v1, v2);
    }

    double score(py::array_t<uint8_t> d1, py::array_t<uint8_t> d2) {
        return score({d1}, {d2});
    }

    py::list get_bow_vector(const std::vector<py::array_t<uint8_t>>& descriptors) {
        std::vector<cv::Mat> vec = to_cv_mat_list(descriptors);
        DBoW2::BowVector v;
        voc.transform(vec, v);

        py::list bow_list;
        for (auto& kv : v) {
            bow_list.append(py::make_tuple(kv.first, kv.second));
        }
        return bow_list;
    }

    py::list get_all_descriptors() const {
        py::list out;
        for (const auto& frame : stored_descriptors) {
            py::list desc_list;
            for (const auto& desc : frame) {
                py::array_t<uint8_t> npy = py::array_t<uint8_t>({desc.rows, desc.cols}, desc.data);
                desc_list.append(npy);
            }
            out.append(desc_list);
        }
        return out;
    }

private:
    Vocabulary voc;
    Database db;
    std::vector<std::vector<cv::Mat>> stored_descriptors;

    std::vector<cv::Mat> to_cv_mat_list(const std::vector<py::array_t<uint8_t>>& numpy_descs) const {
        std::vector<cv::Mat> mats;
        for (const auto& array : numpy_descs) {
            if (array.ndim() != 2 || array.shape(1) != 32) {
                throw std::runtime_error("Each descriptor must be a 2D numpy array with 32 columns (ORB).");
            }
            mats.emplace_back(cv::Mat(array.shape(0), 32, CV_8U, const_cast<uint8_t*>(array.data())).clone());
        }
        return mats;
    }
};

PYBIND11_MODULE(dbow2py, m) {
    py::class_<DBoW2Wrapper>(m, "DBoW2Wrapper")
        .def(py::init<const std::string&>())
        .def("add", py::overload_cast<py::array_t<uint8_t>>(&DBoW2Wrapper::add))
        .def("add", py::overload_cast<const std::vector<py::array_t<uint8_t>>&>(&DBoW2Wrapper::add))
        .def("query", py::overload_cast<py::array_t<uint8_t>, int>(&DBoW2Wrapper::query))
        .def("query", py::overload_cast<const std::vector<py::array_t<uint8_t>>&, int>(&DBoW2Wrapper::query))
        .def("save", &DBoW2Wrapper::save)
        .def("load", &DBoW2Wrapper::load)
        .def("clear", &DBoW2Wrapper::clear)
        .def("size", &DBoW2Wrapper::size)
        .def("score", py::overload_cast<py::array_t<uint8_t>, py::array_t<uint8_t>>(&DBoW2Wrapper::score))
        .def("score", py::overload_cast<const std::vector<py::array_t<uint8_t>>&,
                                         const std::vector<py::array_t<uint8_t>>&>(&DBoW2Wrapper::score))
        .def("get_bow_vector", &DBoW2Wrapper::get_bow_vector)
        .def("get_all_descriptors", &DBoW2Wrapper::get_all_descriptors);
}

// This code defines a Python module `dbow2py` that wraps the DBoW2 library using pybind11.