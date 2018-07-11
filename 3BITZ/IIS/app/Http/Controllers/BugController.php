<?php

namespace App\Http\Controllers;


use App\Bug;
use App\Module;
use App\DataTypes;
use Illuminate\Http\Request;
use DB;
use Illuminate\Support\Facades\Auth;
use Symfony\Component\VarDumper\Cloner\Data;

class BugController extends Controller
{
    public function index()
    {
        $bugs = DB::table('bugs')->join('modules', 'bugs.id_modul', '=', 'modules.id_modul')->get();
        return view('bugs.bugs', compact('bugs'));
    }

    public function create_bug(){
        $modules = Module::all();
        $bug_types = DataTypes::$typ_bugu;
        $vulner  = DataTypes::$vulner;
        $severity = DataTypes::$zavaznost;
        return view('bugs.create', compact('modules', 'bug_types', 'vulner', 'severity'));
    }

    public function show($id_bug)
    {
        $bug_types = DataTypes::$typ_bugu;
        $severity = DataTypes::$zavaznost;
        $bugs = Bug::where('id_bug',$id_bug)->get();
        $vuln = DataTypes::$vulner;
        $modules = Module::all();
        $vulnerab = null;

        if(DB::table('vulnerabilities')->where('id_vulnerability', $bugs[0]->id_bug)->first() != null) {
            $vulnerab = DB::table('vulnerabilities')->where('id_vulnerability', $bugs[0]->id_bug)->first();
        }

        if (sizeof($bugs)){
            $bug = $bugs[0];
            return view('bugs.bug', compact('bug', 'bug_types', 'severity', 'modules', 'vulnerab', 'vuln'));
        } else {
            $bug = null;
            return view('bugs.bug', compact('bug', 'bug_types', 'severity', 'modules', 'vulnerab', 'vuln'));
        }
    }

    public function store(Request $request){
        $validate = $request->validate([
            'popis_problemu' => 'required|max:255',
        ]);

        DB::table('bugs')->where('id_bug', $request->id)->update([
            'typ' => $request->typ,
            'zavaznost' => $request->zavaznost,
            'popis_bugu' => $request->popis_problemu,
            ]);

        if($request->miera_nebezpecenstva)
            DB::table('vulnerabilities')->where('id_vulnerability', $request->id)->update([
                'miera_nebezpecenstva' => $request->miera_nebezpecenstva,
            ]);

        return redirect('/bugs/'.$request->id);
    }

    public function store_bug(Request $request){
        $validate = $request->validate([
            'popis_bugu' => 'required|max:255',
            'zavaznost' => 'required'
        ]);

        $meno_uzivatela = Auth::user()->name;
        $ticket = DB::table('tickets')->where('name_vytvoreny', $meno_uzivatela)->orderBy('id_ticket','desc')->first();
        $modul = $module = Module::where('meno', $request->meno_modulu)->first();

        DB::table('bugs')->insert([
            'typ' => $request->typ,
            'zavaznost' => $request->zavaznost,
            'popis_bugu' => $request->popis_bugu,
            'id_modul' => $modul->id_modul,
        ]);

        $request->id_bug = DB::table('bugs')->orderBy('id_bug', 'desc')->first()->id_bug;
        DB::table('descriptions')->insert([
            'id_ticket' => $ticket->id_ticket,
            'id_bug' => $request->id_bug,
        ]);

        if ($request->miera_nebezpecenstva)
            DB::table('vulnerabilities')->insert([
                'id_vulnerability' => $request->id_bug,
                'miera_nebezpecenstva' => $request->miera_nebezpecenstva
            ]);

        return redirect('/tickets/'.$ticket->id_ticket);
    }


}
