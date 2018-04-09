#pragma once

#include <thread>

namespace logs2 {
    namespace SharedTraits
    {
        class AssertCheck
        {
        public:
            AssertCheck();
            void isSameThread() const;

        protected:
            ~AssertCheck() = default;

        private:
            const std::thread::id _threadId;
        };

        class NoCheck
        {
        public:
            NoCheck() = default;
            void isSameThread() const {}

        protected:
            ~NoCheck() = default;
        };
    }
}
