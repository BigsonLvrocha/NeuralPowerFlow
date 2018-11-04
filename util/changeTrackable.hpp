/**
 * @brief 
 * 
 * @copyright 2018 Luiz Victor Linhares Rocha
 */

#ifndef UTIL_CHANGETRACKABLE_HPP_
#define UTIL_CHANGETRACKABLE_HPP_

namespace neuralFlux {

class ChangeTrackable{
 public:
    ChangeTrackable();
    bool isChanged();
    void clearChanged();
 protected:
    void setChanged();
 private:
    bool m_changed;
};
}  // namespace neuralFlux
#endif  // UTIL_CHANGETRACKABLE_HPP_
