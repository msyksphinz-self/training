/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.vliw.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;
import ru.ispras.microtesk.model.vliw.mode.*;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class C_LE_S extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "C_LE_S",
          C_LE_S.class,
          null
          );
      addArgument("ft", F.INFO);
      addArgument("fs", F.INFO);
      addArgument("cc", COND_CODE);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final F ft = (F) getArgument("ft", args);
      final F fs = (F) getArgument("fs", args);
      final Immediate cc = (Immediate) getArgument("cc", args);

      return new C_LE_S(ft, fs, cc);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final F ft;
  public final F fs;
  public final Immediate cc;

  public C_LE_S(final F ft, final F fs, final Immediate cc) {
    assert F.INFO.isSupported(ft);
    assert F.INFO.isSupported(fs);
    assert COND_CODE.equals(cc.getType());

    this.ft = ft;
    this.fs = fs;
    this.cc = cc;

    addArgument("ft", ft);
    addArgument("fs", fs);
    addArgument("cc", cc);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("C_LE_S %d, %s, %s", cc.access().load().bigIntegerValue(), fs.text(vars__), ft.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000110000%s%s%s00111110", ft.image(vars__), fs.image(vars__), cc.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if ((fs.access(pe__, vars__).load().compareTo(ft.access(pe__, vars__).load()) <= 0)) {
      pe__.FCCR.access().bitField(cc.access().load(), cc.access().load()).store(Data.valueOf(Type.INT(1), 0x1));
    } else {
      pe__.FCCR.access().bitField(cc.access().load(), cc.access().load()).store(Data.valueOf(Type.INT(1), 0x0));
    }
  }
}