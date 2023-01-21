//
//  ContentView.swift
//  autoLightOff
//
//  Created by 渡邉雅晃 on 2022/12/22.
//

import SwiftUI

struct ContentView: View {
    @State var isMain = true;
    @ObservedObject var notifycationModel = NotifycationModel()
    
    init(){
        notifycationModel.askNotifycationPermit()
    }
    
    var body: some View {
        NavigationView(content: {
            MainView()
        })
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
