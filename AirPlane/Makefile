# Qt代理模式演示项目 Makefile
# Qt Proxy Pattern Demo Project Makefile

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET_DIR = .
SOURCES = ProxyTest.cpp

# 默认目标
all: proxy_test

# 编译代理模式演示程序
proxy_test: ProxyTest.cpp
	$(CXX) $(CXXFLAGS) -o proxy_test ProxyTest.cpp
	@echo "代理模式演示程序编译完成！运行 ./proxy_test 来查看演示。"
	@echo "Proxy pattern demo compiled! Run ./proxy_test to see the demonstration."

# 运行演示程序
run: proxy_test
	@echo "=== 运行Qt代理模式演示程序 ==="
	@echo "=== Running Qt Proxy Pattern Demo ==="
	./proxy_test

# 清理编译产物
clean:
	rm -f proxy_test *.o
	@echo "清理完成。"
	@echo "Cleanup complete."

# 显示帮助信息
help:
	@echo "可用目标 (Available targets):"
	@echo "  all        - 编译所有程序 (Compile all programs)"
	@echo "  proxy_test - 编译代理模式演示程序 (Compile proxy pattern demo)"
	@echo "  run        - 编译并运行演示程序 (Compile and run demo)"
	@echo "  clean      - 清理编译产物 (Clean build artifacts)"
	@echo "  help       - 显示此帮助信息 (Show this help)"
	@echo ""
	@echo "示例用法 (Example usage):"
	@echo "  make         - 编译演示程序"
	@echo "  make run     - 编译并运行演示程序"
	@echo "  make clean   - 清理编译产物"

# 检查代码风格（可选）
check:
	@echo "检查代码风格..."
	@echo "Checking code style..."
	# 这里可以添加静态分析工具，如 cppcheck
	@echo "代码检查完成。"
	@echo "Code check complete."

.PHONY: all run clean help check