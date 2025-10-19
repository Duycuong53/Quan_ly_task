# FreeRTOS Queue Demo – Điều khiển LED bằng Queue (STM32F103)

## 📝 Giới thiệu

Chương trình minh họa cách sử dụng **FreeRTOS Queue** để **truyền dữ liệu giữa hai task** trên vi điều khiển **STM32F103**.  
Một task đảm nhận **tạo dữ liệu điều khiển (tần số và độ rộng xung)**, task còn lại **nhận dữ liệu qua Queue và điều khiển LED** theo các giá trị đó.

---

## ⚙️ Mô tả hoạt động

- **Task 1 – `LED_Task`**  
  Nhận cấu trúc dữ liệu `BlinkData_t` (gồm tần số & duty cycle) từ Queue.  
  LED sẽ nhấp nháy theo thông tin nhận được:
  - `frequency`: tần số nhấp nháy (Hz)  
  - `duty`: độ rộng xung (%)  

- **Task 2 – `Gen_Task`**  
  Sinh dữ liệu thay đổi tần số và duty theo thời gian.  
  Gửi dữ liệu vào Queue mỗi 1 giây để task LED cập nhật.  

- **Cơ chế Queue**  
  - Dung lượng: 5 phần tử  
  - Kiểu dữ liệu: `BlinkData_t`  
  - Đảm bảo truyền thông an toàn giữa các task trong FreeRTOS.

---

## 💡 Chu kỳ nhấp nháy LED

| Tham số | Ý nghĩa | Ví dụ |
|----------|----------|--------|
| `frequency` | Tần số nhấp nháy (Hz) | 1 → LED nháy 1 lần/giây |
| `duty` | Độ rộng xung (%) | 50 → LED sáng 50%, tắt 50% |

Trong quá trình chạy:
- Tần số thay đổi từ **1 Hz → 5 Hz**, sau đó quay lại 1 Hz.  
- Duty thay đổi từ **30 % → 90 %**, sau đó quay lại 30 %.  

---

## 🔩 Phần cứng sử dụng

- **Vi điều khiển:** STM32F103C8T6  
- **LED:** gắn vào chân PC13 (LED_BUILTIN trên board Blue Pill)  
- **Điện trở hạn dòng:** 220 Ω  
- **Nguồn:** 3.3 V  

| Chân STM32 | Thiết bị | Ghi chú |
|-------------|-----------|---------|
| PC13 | LED | Active LOW (0 = Sáng, 1 = Tắt) |

---

## 🧠 Cấu trúc dữ liệu

```c
typedef struct {
    float frequency;   // Tần số LED (Hz)
    float duty;        // Độ rộng xung (%)
} BlinkData_t;
