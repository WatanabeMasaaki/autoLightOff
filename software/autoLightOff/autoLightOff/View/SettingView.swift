//
//  SettingView.swift
//  autoLightOff
//
//  Created by 渡邉雅晃 on 2022/12/22.
//

import SwiftUI
import Foundation

extension Int {
    var data: Data {
        var int = self
        return Data(bytes: &int, count: MemoryLayout<UInt32>.size)
    }
}

struct SettingView: View {
    @ObservedObject var bleModel: BLEModel
    @Environment(\.presentationMode) var presentationMode: Binding<PresentationMode>
    @State var hour: Int
    @State var min: Int

    var body: some View {
        VStack{
            Spacer()
            
            Text("時刻設定")
                .font(.title)
                .bold()
            
//            let setMinStr = min < 10 ? "0" + String(min) : String(min)
//            Text(" \(hour):\(setMinStr) ")
//                .foregroundColor(.white)
//                .font(.system(size:128))
//                .background(Color.blue.cornerRadius(40))
                
            Button(action: {
                MainView().setHour = hour
                MainView().setMin = min
                self.presentationMode.wrappedValue.dismiss()
                
                let timeData : String = String(hour * 60 + min)
                let data : Data = timeData.data(using: .utf8)!
                bleModel.sendData(data: data)
            }, label: {
                VStack{
                    let setMinStr = min < 10 ? "0" + String(min) : String(min)
                    Text("\(hour):\(setMinStr)  ")
                        .foregroundColor(.white)
                        .font(.system(size:100))
                        .background(Color.blue.cornerRadius(40))
                        
                    Text("保存して戻る")
                        .font(.title)
                }
            })
                   
            
            Spacer()
            
            HStack{
                Picker(selection: $hour){
                    ForEach(0..<24){ index in
                        Text(String(index))
                            .tag(index)
                    }
                } label: {
                    Text("時間")
                }
                .pickerStyle(.wheel)
                
                Picker(selection: $min){
                    ForEach(0..<60){ index in
                        Text(String(index))
                            .tag(index)
                    }
                } label: {
                    Text("分")
                }
                .pickerStyle(.wheel)
            }
        }
    }
}

//struct SettingView_Previews: PreviewProvider {
//    static var previews: some View {
//        SettingView(hour: 23, min: 1)
//    }
//}
