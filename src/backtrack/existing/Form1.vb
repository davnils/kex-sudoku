Public Class Form1

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim itime As DateTime = DateTime.Now
        Dim l As Long
        Dim target As Integer = 2000
        For i As Integer = 1 To target
            SudoGen.GenerateGrid()
            Me.Text = i
        Next
        l = DateDiff(DateInterval.Second, itime, DateTime.Now)
        Me.Text = l / target
        '------iterations above ----------- single paint below ----------
        'sudoGen.GenerateGrid()
        'Dim g As Graphics = Me.CreateGraphics
        'g.FillRectangle(New SolidBrush(Color.FromKnownColor(KnownColor.Control)), 5, 5, 267, 267)
        'For Each s As SudoGen.Square In SudoGen.Sudoku
        '    g.DrawString(s.Value, Me.Font, Brushes.Black, 10 + (s.Across * 8) + ((s.Across - 1) * Me.Font.Size), 10 + (s.Down * 8) + ((s.Down - 1) * Me.Font.Size))
        'Next
    End Sub

End Class
