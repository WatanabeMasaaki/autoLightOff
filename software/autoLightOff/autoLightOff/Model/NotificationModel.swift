//
//  NotificationModel.swift
//  autoLightOff
//
//  Created by 渡邉雅晃 on 2023/01/21.
//

import Foundation
import UserNotifications

class NotificationModel: ObservableObject {
    
    func askNotificationPermit() {
        UNUserNotificationCenter.current().requestAuthorization(options: [.alert, .badge, .sound]){
            (granted, _) in
            if granted {
                //通知が許可されているときの処理
                print("通知が許可されました！")
            }else {
                //通知が拒否されているときの処理
                print("通知が許可されませんでした")
            }
        }
    }
    
    func makeNotification() {
        let trigger = UNTimeIntervalNotificationTrigger(timeInterval: 5, repeats: false)
        
        let content = UNMutableNotificationContent()
        content.title = "ローカル通知"
        content.body = "ローカル通知を発行しました"
        content.sound = UNNotificationSound.default
        
        let request = UNNotificationRequest(identifier: "notification001", content: content, trigger: trigger)
        UNUserNotificationCenter.current().add(request, withCompletionHandler: nil)
        
    }
}


